
#include <iostream>
#include <utility>

#include "../base/TimerWheel.h"

TimerWheel::TimerWheel() 
    : defaultTimerCallBack_(std::bind(&TimerWheel::defaultTimerCallBack, 
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
                              timerCallBack cb = nullptr) 
{
    if (cb == nullptr) 
    {
        cb = defaultTimerCallBack_;
    }
    if (firstTime < 0 || interval < 0) return;
    
    //多少次SI后被触发
    int timeout = (firstTime < SI) ? 1 : (firstTime / SI);
    //转动多少圈后被触发
    int rotation = timeout / N;
    //应该插入那个槽
    int ts = (currentSlot + (timeout % N)) % N;

    Timer timerTmp(firstTime, interval, cb);
    uint32_t timerID = timerTmp.id();
    //插入该槽
    wheel[ts].emplace(std::move(timerTmp));

    return timerID;
}

void TimerWheel::cancleTimer(uint32_t timerID) 
{
    for (int i = 0; i < N; i++) 
    {
        wheel[i].erase(timerID);
    }
}


void TimerWheel::tick() 
{

}

void TimerWheel::reSet() 
{

}

void TimerWheel::defaultTimerCallBack(int fd = -1) 
{
    std::cout << "performing defaultTimerCallBack "
              << " and Timer`s fd = "
              << fd 
              << std::endl;
}