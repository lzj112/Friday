#ifndef TIMER_H_
#define TIMER_H_

#include <unistd.h>
#include <assert.h>

#include <functional>

#include "../base/UniqueID.h"

typedef std::function<void(int)> timerCallBack;

//使用相对时间(epoch时间)
class Timer 
{
public:
    Timer(timerCallBack cb, int firstTime, int interval);
    ~Timer() {}
    void tick();
    void reStart();
    int expiration() const  { return expire; }
    bool isReapt() { return isRepeat_; }
    void shutdown();
    int setFd(int fd) { mayFd = fd; }
    int fd() { return mayFd; }

    bool operator<(const Timer& tmp) const
    {
        return expire > tmp.expiration();
    }

private:
    
    int mayFd;
    uint32_t timerID;   //定时器ID 全局唯一
    int expire;      //到期时间
    int interval;    //时间间隔

    bool isRepeat_;
    timerCallBack timeCB;

};

#endif