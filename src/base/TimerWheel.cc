
#include <signal.h>
#include <errno.h>

#include <utility>
#include <string>
#include <iostream>
#include <exception>

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
    location.emplace(std::make_pair(timerID, std::move(timerTmp)));
    //插入该槽
    wheel[ts].emplace(std::move(timerTmp));

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
    if (!wheel[currentSlot].empty()) 
    {
        auto it = wheel[currentSlot].begin();
        auto end = wheel[currentSlot].end();
        timespec now;

        for (; it != end;) 
        {
            clock_gettime(CLOCK_REALTIME, &now);
            int timeNow = static_cast<int> (now.tv_sec);
            int timeStamp = it->expiration();

            if (timeStamp <= timeNow) 
            {
                it->tick();
                if (it->isRepeat()) 
                {
                    int expire = it->interval();
                    int interval = it->interval();
                    int fd = it->fd();
                    timerCallBack cb = it->timerFunc();

                    it = wheel[currentSlot].erase(it);

                    addTimer(expire,
                             interval,
                             cb,
                             fd);
                }
                else 
                {
                    it = wheel[currentSlot].erase(it);
                }
            }
            else 
            {
                it++;
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
