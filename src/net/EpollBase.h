#ifndef EPOLLBASE_H_
#define EPOLLBASE_H_

#include <unistd.h>
#include <sys/epoll.h>

#include <vector>
#include <functional>

typedef std::function<int(int, const char*, int)> IOcallBack;
typedef std::function<void(int)> closeCallBack; //关闭连接直接执行,不放进任务队列

struct MyEvent 
{
    int fd;
    int events;
    int timerfd;

    IOcallBack readCallBack_;
    IOcallBack writeCallBack_;
    closeCallBack closeCallBack_;

    void* ptr;
};

class EpollBase 
{
public:
    EpollBase() { epollFd = epoll_create(1); }
    ~EpollBase() { close(epollFd); }
    int fd() { return epollFd; }
    void add(MyEvent* event, epoll_event* ev);
    void del(MyEvent* event, epoll_event* ev);
    void ctl(MyEvent* event, epoll_event* ev);
    void wait(std::vector<epoll_event>& readyEvents, int timeout = 0);
private:
    int epollFd;
};


#endif