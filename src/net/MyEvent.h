#ifndef MYEVENT_H_
#define MYEVENT_H_

#include <unistd.h>

#include <functional>
#include <sys/epoll.h>

typedef std::function<int(int)> IOcallBack;
typedef std::function<void(int)> closeCallBack; //关闭连接直接执行,不放进任务队列

enum EPOLLEVENTS
{
    pollPri = EPOLLPRI,
    pollErr = EPOLLERR,
    pollLevelTrigger = 0,
    pollHangUp = EPOLLHUP,
    pollReadAble = EPOLLIN,
    pollWriteAble = EPOLLOUT,
    pollEdgeTrigger = EPOLLET,
    pollRDHangUp = EPOLLRDHUP,
    pollOneShot = EPOLLONESHOT,
};

//针对每个连接封装其属性和操作

class MyEvent 
{
public:
    MyEvent();
    MyEvent(int fd, int timeout);
    ~MyEvent() {}
    
    void setFd(int fd) { fd_ = fd; }
    void setTimer(int timer) { timerfd_ = timer; }

    int fd() { return fd_; }
    int timefd() { return timerfd_; }

    void setReadCallBack(const IOcallBack& cb) 
    { readCallBack_ = cb; }
    void setWriteCallBack(const IOcallBack& cb) 
    { writeCallBack_ = cb; }
    void setCloseCallBack(const closeCallBack& cb) 
    { closeCallBack_ = cb; }

    int defRead(int);
    int defWrite(int);
    void defClose(int) 
    {
        //log TODO
        ::close(fd_);
        fd_ = -1;
    }

    void goRead(int fd) { readCallBack_(fd); }
    void goWrite(int fd) { writeCallBack_(fd); }
    void goClose(int fd) { closeCallBack(fd); }
    
private:
    int fd_;
    int timerfd_;
    void* ptr;
    
    IOcallBack readCallBack_;
    IOcallBack writeCallBack_;
    closeCallBack closeCallBack_;
};

#endif
