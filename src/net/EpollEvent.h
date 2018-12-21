#ifndef EPOLLEVENT_H_
#define EPOLLEVENT_H_

#include <sys/epoll.h>

#include <map>
#include <vector>
#include <functional>

#include "../base/FileDes.h"
#include "../net/EpollBase.h"

typedef std::function<int(int, const char*, int)> IOcallBack;
typedef std::function<void(int)> closeCallBack;

static const int INITMAXEVENTS = 1024;


struct MyEvent 
{
    // int fd;
    FileDes fd;
    int events;
    int timerfd;

    IOcallBack readCallBack_;
    IOcallBack writeCallBack_;
    closeCallBack closeCallBack_;

    void* ptr;
};

class EpollEvent 
{
public:
    EpollEvent();
    EpollEvent(IOcallBack& read, IOcallBack& write, closeCallBack& close);
    ~EpollEvent();

    void setReadCallBack(IOcallBack& cb) { readCallBack_ = cb; }
    void setWriteCallBack(IOcallBack& cb) { writeCallBack_ = cb; }
    void setCloseCallBack(closeCallBack& cb) { closeCallBack_ = cb; }

    void updateReadEvents();

    void updateWriteEvents();
    void delEvents();

    void poll(std::vector<epoll_event>& events);

private:
    EpollBase epollFd;

    int eventsSize;
    std::vector<epoll_event> events;
    std::map<int, epoll_event> eventsMap;

    IOcallBack readCallBack_;
    IOcallBack writeCallBack_;
    closeCallBack closeCallBack_;
};

#endif