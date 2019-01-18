#ifndef TIMERLWHEEL_H_
#define TIMERLWHEEL_H_

#include <set> 
#include <map>
#include <vector>

#include "Timer.h"

class TimerWheel
{
public:
    TimerWheel();
    ~TimerWheel();

    int tickTime() const { return SI; }
    uint32_t addTimer(int firstTime, int interval, timerCallBack cb, int fd);
    void cancleTimer(uint32_t timerID);
    void tick();

    void show();

private:
    void defaultTimerCallBack(int);
    timerCallBack defaultTimerCallBack_;

    static const int N = 10;
    static const int SI = 2;

    std::vector<std::set<Timer> > wheel;
    std::map<uint32_t, Timer> location;
    int currentSlot;
};

#endif