
#include "../base/Timer.h"

Timer::Timer(int firstTime, int interval, timerCallBack cb) 
    : timerID(UniqueID().ID()),
      expire(-1),
      interval_((interval > 0) ? interval : 0),
      isRepeat_(interval > 0),
      timeCB(cb),
      myFd(-1)
{
    timespec now;
    clock_gettime(CLOCK_REALTIME, &now);
    expire = static_cast<int> (now.tv_sec) + firstTime;
}

void Timer::tick() 
{
    timeCB(myFd); //执行回调函数
}

void Timer::reStart() 
{
    if (interval_ != 0) 
    {
        expire = expire + interval_;
    }
}

Timer::Timer(Timer&& tmp) 
    : myFd(tmp.fd()),
      timerID(tmp.id()),
      expire(tmp.expiration()),
      interval_(tmp.interval()),
      isRepeat_(tmp.isRepeat()),
      timeCB(tmp.timerFunc())
{}

Timer& Timer::operator=(Timer&& tmp) 
{
    if (this != &tmp) 
    {
        myFd = tmp.fd();
        timerID = tmp.id();
        timeCB = tmp.timerFunc();
        expire = tmp.expiration();
        interval_ = tmp.interval();
        isRepeat_ = tmp.isRepeat();
    }
    return *this;
}
