#ifndef TIMERLWHEEL_H_
#define TIMERLWHEEL_H_

#include <set> 
#include <vector>

#include "Timer.h"

class TimerWheel
{
public:
    TimerWheel();
    ~TimerWheel();

    uint32_t addTimer(int firstTime, int interval, timerCallBack cb);
    void cancleTimer(uint32_t timerID);
    void tick();

private:
    void reSet();
    void defaultTimerCallBack(int);
    timerCallBack defaultTimerCallBack_;

    static const int N = 10;
    static const int SI = 1;

    std::vector<std::set<Timer> > wheel;
    int currentSlot;
};

#endif