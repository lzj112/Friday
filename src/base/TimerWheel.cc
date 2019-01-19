
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

// std::cout << "插入前****\n";
//     show();
    
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
    // Timer timerTmp(10, 3, cb, 8);
    uint32_t timerID = timerTmp.id();
    //记录timerid和定时器指针的对应关系
    // location.emplace(std::make_pair(timerID, std::move(timerTmp)));
    // location.emplace(std::make_pair(timerID, timerTmp));
        auto iter = location.insert(std::make_pair(timerID, timerTmp));
    //插入该槽
    // wheel[ts].emplace(std::move(timerTmp));
    // auto ret = wheel[ts].emplace(timerTmp);
        auto r = wheel[ts].find(timerTmp);
        if (r != wheel[ts].end()) 
        {
            std::cout << "竟然有 你敢信******************" << std::endl;
            std::cout << "TimerTmp = " << timerTmp.fd() << ' '
                      << timerTmp.expiration() << ' '
                      << timerTmp.interval() << ' '
                      << timerTmp.id() << ' '
                      << timerTmp.isRepeat() << std::endl;
        for (auto it = wheel[ts].begin(), end = wheel[ts].end(); it != end; it++) 
        {
            std::cout << "第　" << ts << " 槽 = ";
            std::cout << it->fd() << ' '
                      << it->expiration() << ' '
                      << it->interval() << ' '
                      << it->id() << ' '
                      << it->isRepeat() << std::endl;
        }
        }
        auto ret = wheel[ts].insert(timerTmp);
    if (ret.second == false) 
    {
        std::cout << ret.first->fd() << " 没有发生插入!!! " << std::endl;
        std::cout << timerTmp.fd() << " 没有发生插入!!! " << std::endl;
    }

std::cout << fd << " 应该插入 " << ts << " 槽" << std::endl;

// std::cout << "插入后====\n";
//     show();
    // for (auto it = wheel[ts].begin(), end = wheel[ts].end(); it != end; it++) 
    //     {
    //         std::cout << "第　" << ts << " 槽\n";
    //         std::cout << it->expiration() << ' ' << it->fd() << std::endl;
    //     }

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
    std::cout << "有定时事件,当前槽 = " << currentSlot << std::endl;
            clock_gettime(CLOCK_REALTIME, &now);
            int timeNow = static_cast<int> (now.tv_sec);
            int timeStamp = it->expiration();

            if (timeStamp <= timeNow) 
            git {
                it->tick();
                if (it->isRepeat()) 
                {
            std::cout << "重新添加 " << it->fd() << std::endl;
                    int expire = it->interval();
                    int interval = it->interval();
                    int fd = it->fd();
                    timerCallBack cb = it->timerFunc();

                    // it = wheel[currentSlot].erase(it);
                    auto delit = it;
                    wheel[currentSlot].erase(delit);
                    it++;

                    addTimer(expire,
                             interval,
                             cb,
                             fd);
                }
                else 
                {
                    // it = wheel[currentSlot].erase(it);
                    auto delit = it;
                    wheel[currentSlot].erase(delit);
                    it++;
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

void TimerWheel::show() 
{
    for (int i = 0; i < N; i++) 
    {
        for (auto it = wheel[i].begin(), end = wheel[i].end(); it != end; it++) 
        {
            std::cout << "第　" << i << " 槽\n";
            std::cout << it->expiration() << ' ' << it->fd() << std::endl;
        }
    }
}