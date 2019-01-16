
#include "../base/Timer.h"

Timer::Timer(int firstTime, int interval, timerCallBack cb) 
    : timerID(UniqueID().ID()),
      expire(firstTime),
      interval((interval > 0) ? interval : 0),
      isRepeat_(interval > 0),
      timeCB(cb),
      myFd(-1)
{
}

void Timer::tick() 
{
    timeCB(myFd); //执行回调函数
}

void Timer::reStart() 
{

}
