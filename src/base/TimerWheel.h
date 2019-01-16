#ifndef TIMERLWHEEL_H_
#define TIMERLWHEEL_H_

#include <map> 
#include <vector>
#include <queue>

#include "Timer.h"

class TimerWheel
{
public:
    TimerWheel();
    ~TimerWheel();

    uint32_t addTimer(int firstTime, int interval, timerCallBack cb);
    void cancelTimer(Timer* timer);
    void tick();

private:
    void reSet();
    void defaultTimerCallBack(int);
    timerCallBack defaultTimerCallBack_;

    static const int N = 10;
    static const int SI = 1;

    std::vector<std::priority_queue<Timer> > wheel;
    int currentSlot;
};

#endif