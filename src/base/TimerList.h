#ifndef TIMERLIST_H_
#define TIMERLIST_H_

#include <map> 

#include "Timer.h"

class TimerList
{
public:
    TimerList();
    ~TimerList();

    

private:
    std::map<int, Timer> timers;
};

#endif