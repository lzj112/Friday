
#include <signal.h>

#include <utility>
#include <string>
#include <iostream>

#include "../base/TimerWheel.h"

TimerWheel::TimerWheel() 
    : currentSlot(0),
      defaultTimerCallBack_(std::bind(&TimerWheel::defaultTimerCallBack, 
                                      this,
                                      std::placeholders::_1))
{
    wheel.resize(N);
}

TimerWheel::~TimerWheel() 
{
    
}

uint32_t TimerWheel::addTimer(int firstTime, 
                              int interval, 
                              timerCallBack cb = nullptr,
                              int fd = -1) 
{
std::cout << "插入前****\n";
    show();
    
    if (cb == nullptr) 
    {
        cb = defaultTimerCallBack_;
    }
    if (firstTime < 0 || interval < 0) return -1;
    
    //多少次SI后被触发
    int timeout = (firstTime < SI) ? 1 : (firstTime / SI);
    // //转动多少圈后被触发
    // int rotation = timeout / N;
    //应该插入哪个槽
    int ts = (currentSlot + (timeout % N)) % N;

    Timer timerTmp(firstTime, interval, cb, fd);
    uint32_t timerID = timerTmp.id();
    //记录timerid和定时器指针的对应关系
    // location.emplace(std::make_pair(timerID, std::move(timerTmp)));
    location.emplace(std::make_pair(timerID, timerTmp));
    //插入该槽
    // wheel[ts].emplace(std::move(timerTmp));
    wheel[ts].emplace(timerTmp);

std::cout << fd << " 应该插入 " << ts << " 槽" << std::endl;

std::cout << "插入后====\n";
    show();

    return timerID;
}

void TimerWheel::cancleTimer(uint32_t timerID) 
{
    auto it = location.find(timerID);
    if (it != location.end()) 
    {
        for (int i = 0; i < N; i++) 
        {
            auto iter = wheel[i].find(it->second);
            wheel[i].erase(iter);
        }
    }
    return ;
}


void TimerWheel::tick() 
{
std::cout << "检查" << std::endl;
    if (!wheel[currentSlot].empty()) 
    {
        auto it = wheel[currentSlot].begin();
        auto end = wheel[currentSlot].end();
        timespec now;

        for (; it != end;) 
        {
    std::cout << "有定时事件,当前槽 = " << currentSlot << std::endl;
            clock_gettime(CLOCK_REALTIME, &now);
            int timeNow = static_cast<int> (now.tv_sec);
            int timeStamp = it->expiration();

            if (timeStamp <= timeNow) 
            {
                it->tick();
                if (it->isRepeat()) 
                {
            std::cout << "重新添加 " << it->fd() << std::endl;
                    addTimer(it->interval(),
                             it->interval(),
                             it->timerFunc(),
                             it->fd());
                    it = wheel[currentSlot].erase(it);
                }
                else 
                    it = wheel[currentSlot].erase(it);
            }
        }
    }

    currentSlot = ++currentSlot % N;    
}

void TimerWheel::defaultTimerCallBack(int fd = -1) 
{
    std::cout << "performing defaultTimerCallBack "
              << " and Timer`s fd = "
              << fd 
              << std::endl;
}

void TimerWheel::show() 
{
    for (int i = 0; i < N; i++) 
    {
        std::cout << "第　" << i << " 槽\n";
        for (auto it = wheel[i].begin(), end = wheel[i].end(); it != end; it++) 
        {
            std::cout << it->expiration() << ' ' << it->fd() << std::endl;
        }
        std::cout << std::endl;
    }
}