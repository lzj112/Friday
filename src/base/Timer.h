#pragma once

#include <unistd.h>
#include <assert.h>
#include <sys/time.h>
#include <sys/timerfd.h>

#include <functional>
#include <iostream>
 
#include "UniqueID.h"

typedef std::function<void(void)> timerCallBack;

//使用相对时间(epoch时间)
class Timer 
{
public:
    Timer(int firstTime, int interval, timerCallBack cb);
    Timer(Timer&& tmp) noexcept;
    Timer(const Timer& t);
    ~Timer() {}
    void tick() const;
    int expiration() const  { return expire; }
    int interval() const { return interval_; }
    bool isRepeat() const { return isRepeat_; }
    void shutdown() { expire = -1; interval_ = 0; }
    timerCallBack timerFunc() const { return timeCB; }
    uint32_t id() const { return timerID; }

    bool operator<(const Timer& tmp) const;
    
    bool operator==(const Timer& tmp) const;
    Timer& operator=(const Timer& tmp);
    Timer& operator=(Timer&& tmp);

private:
    
    // int myFd;
    int expire;      //到期时间
    int interval_;    //时间间隔
    uint32_t timerID;   //定时器ID 全局唯一

    bool isRepeat_;
    timerCallBack timeCB;

};
