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
    Timer(int firstTime, int interval, timerCallBack cb);
    Timer(Timer&& )
    ~Timer() {}
    void tick();
    void reStart();
    int expiration() const  { return expire; }
    bool isReapt() { return isRepeat_; }
    void shutdown() { expire = -1; interval = 0; }
    int setFd(int fd) { myFd = fd; }
    int fd() { return myFd; }
    uint32_t id() { return timerID; }

    bool operator<(const Timer& tmp) const
    {
        return expire > tmp.expiration();
    }

private:
    
    int myFd;
    uint32_t timerID;   //定时器ID 全局唯一
    int expire;      //到期时间
    int interval;    //时间间隔

    bool isRepeat_;
    timerCallBack timeCB;

};

#endif