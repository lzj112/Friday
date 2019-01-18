#ifndef TIMER_H_
#define TIMER_H_

#include <unistd.h>
#include <assert.h>
#include <sys/timerfd.h>

#include <functional>
 
#include "../base/UniqueID.h"

typedef std::function<void(int)> timerCallBack;

//使用相对时间(epoch时间)
class Timer 
{
public:
    Timer(int firstTime, int interval, timerCallBack cb, int fd);
    // Timer(Timer&& tmp);
    ~Timer() {}
    void tick() const;
    int expiration() const  { return expire; }
    int interval() const { return interval_; }
    bool isRepeat() const { return isRepeat_; }
    void shutdown() { expire = -1; interval_ = 0; }
    int setFd(int fd) { myFd = fd; }
    int fd() const { return myFd; }
    timerCallBack timerFunc() const { return timeCB; }
    uint32_t id() const { return timerID; }

    bool operator<(const Timer& tmp) const
    {
        return expire < tmp.expiration();
    }
    // Timer& operator=(Timer&& tmp);

private:
    
    int myFd;
    uint32_t timerID;   //定时器ID 全局唯一
    int expire;      //到期时间
    int interval_;    //时间间隔

    bool isRepeat_;
    timerCallBack timeCB;

};

#endif