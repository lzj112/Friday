#ifndef EPOLLBASE_H_
#define EPOLLBASE_H_

#include <sys/epoll.h>


class EpollBase 
{
public:
    EpollBase() { epollFd = epoll_create(1); }
    ~EpollBase();
    int fd() { return epollFd; }
    void Add(MyEvent* ev);
    void Del(MyEvent* ev);
    void Ctl(MyEvent* ev);
    void Wait(std::vector<epoll_event>& readyEvents);
private:
    int epollFd;
};


#endif