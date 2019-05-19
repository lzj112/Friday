
#pragma once

#include <sys/timerfd.h>

#include <set> 
#include <map>
#include <vector>
#include <functional>

#include "Timer.h"
#include "FileDes.h"
#include "EpollEventLoop.h"



class MyEvent;
class EpollEventLoop;

class TimerWheel 
{
public:
    TimerWheel(EpollEventLoop* loop);
    ~TimerWheel();

    int tickTime() const { return SI; }
    uint32_t addTimer(int firstTime, 
                      int interval, 
                      timerCallBack cb = nullptr);
    void cancleTimer(uint32_t timerID);
    void tick();

private:
    void start();
    void readTimerfd();
    void defaultTimerCallBack();

private:
    static const int N = 10;
    static const int SI = 2;
    timerCallBack defaultTimerCallBack_;

    EpollEventLoop* loop_;
    std::vector<std::set<Timer> > wheel;
    std::map<uint32_t, Timer> location;
    int currentSlot;

    FileDes timerfd_;

};
