
#include "Timer.h"

Timer::Timer(int firstTime, int interval, timerCallBack cb, int fd = -1) 
    : timerID(UniqueID().ID()),
      expire(-1),
      interval_((interval > 0) ? interval : 0),
      isRepeat_(interval > 0),
      timeCB(cb),
      myFd(fd)
{
    timespec now;
    clock_gettime(CLOCK_REALTIME, &now);
    expire = static_cast<int> (now.tv_sec) + firstTime;
}

void Timer::tick() const
{
    timeCB(); //执行回调函数
}

Timer& Timer::operator=(const Timer& tmp) 
{
    if (this == &tmp) 
        return *this;
    myFd = tmp.fd();
    timerID = tmp.id();
    expire = tmp.expiration();
    interval_ = tmp.interval();
    isRepeat_ = tmp.isRepeat();
    timeCB = tmp.timerFunc();
    // myFd = tmp.myFd;
    // timerID = tmp.timerID;
    // expire = tmp.expire;
    // interval_ = tmp.interval_;
    // isRepeat_ = tmp.isRepeat_;
    // timeCB = tmp.timeCB;
}

// Timer::Timer(Timer&& tmp) noexcept
//     : myFd(tmp.fd()),
//       timerID(tmp.id()),
//       expire(tmp.expiration()),
//       interval_(tmp.interval()),
//       isRepeat_(tmp.isRepeat()),
//       timeCB(tmp.timerFunc())
// {}

bool Timer::operator<(const Timer& tmp) const
{
    if (expire < tmp.expiration()) 
    {
        return true;
    }
    else if (expire > tmp.expiration()) 
    {
        return false;
    }
    else if (expire == tmp.expiration()) 
    {
        if (timerID != tmp.id() || myFd != tmp.fd()) 
        {
            return true;
        }
        else 
        {
            return false;
        }
    }
}

bool Timer::operator==(const Timer& tmp) const 
{
    if (myFd == tmp.fd() &&
        timerID == tmp.id() &&
        expire == tmp.expiration() && 
        interval_ == tmp.interval())
    {
        std::cout << "they are same" << std::endl;
        return true;
    }
    else 
    {
        std::cout << "they are not" << std::endl;
        return false;
    }
}

// Timer& Timer::operator=(Timer&& tmp) 
// {
//     if (this != &tmp) 
//     {
//         myFd = tmp.fd();
//         timerID = tmp.id();
//         timeCB = tmp.timerFunc();
//         expire = tmp.expiration();
//         interval_ = tmp.interval();
//         isRepeat_ = tmp.isRepeat();
//     }
//     return *this;
// }
