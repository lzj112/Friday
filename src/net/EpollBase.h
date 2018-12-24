#ifndef EPOLLBASE_H_
#define EPOLLBASE_H_

#include <unistd.h>
#include <sys/epoll.h>

#include <vector>
#include <functional>

typedef std::function<int(int)> IOcallBack;
typedef std::function<void(int)> closeCallBack; //关闭连接直接执行,不放进任务队列

class MyEvent 
{
public:
    MyEvent() : fd_(-1), events_(-1), timerfd_(-1) {}
    MyEvent(int fd, int events, int event, int timeout) 
        : fd_(fd),
          events_(events),
          timerfd_(timeout)
    {}
    ~MyEvent() {}
    
    void setFd(int fd) { fd_ = fd; }
    void setEvent(int event) { events_ = event; }
    void setTimer(int timer) { timerfd_ = timer; }

    int fd() { return fd_; }
    int event() { return events_; }
    int timefd() { return timerfd_; }

    void setReadCallBack(const IOcallBack& cb) 
    { readCallBack_ = cb; }
    void setWriteCallBack(const IOcallBack& cb) 
    { writeCallBack_ = cb; }
    void setCloseCallBack(const closeCallBack& cb) 
    { closeCallBack_ = cb; }
    
private:
    int fd_;
    int events_;
    int timerfd_;
    void* ptr;

    IOcallBack readCallBack_;
    IOcallBack writeCallBack_;
    closeCallBack closeCallBack_;
};

enum EPOLLEVENTS
{
    pollPri = EPOLLPRI,
    pollErr = EPOLLERR,
    pollLevelTrigger = 0,
    pollHangUp = EPOLLHUP,
    pollReadAble = EPOLLIN,
    pollWriteAble = EPOLLOUT,
    pollEdgeTrigger = EPOLLET,
    pollOneShot = EPOLLONESHOT,
};

class EpollBase 
{
public:
    EpollBase() { epollFd = epoll_create(1); }
    ~EpollBase() { close(epollFd); }
    int fd() { return epollFd; }
    void add(int fd, epoll_event* ev);
    void del(int fd, epoll_event* ev);
    void ctl(int fd, epoll_event* ev);
    void wait(std::vector<epoll_event>& readyEvents, int timeout = 0);
private:
    int epollFd;
};


#endif