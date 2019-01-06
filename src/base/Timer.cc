
#include "../base/Timer.h"

Timer::Timer(timerCallBack cb, int firstTime, int interval) 
    : timerID(UniqueID().ID()),
      expire(firstTime),
      interval((interval > 0) ? interval : 0),
      isRepeat_(interval > 0),
      timeCB(cb),
      mayFd(-1)
{
}

void Timer::tick() 
{
    timeCB(mayFd);
}
