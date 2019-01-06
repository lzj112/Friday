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

    void addTimer(timerCallBack cb, int firstTime, int interval);
    void cancelTimer(Timer* timer);

private:
    void reSet();

    static const int N = 60;
    static const int SI = 1;

    std::vector<std::priority_queue<Timer> > wheel;
    int currentSlot;
};

#endif