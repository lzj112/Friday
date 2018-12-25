#ifndef EPOLLBASE_H_
#define EPOLLBASE_H_

#include <unistd.h>
#include <sys/epoll.h>

#include <vector>
#include <functional>

//封装epoll基础API

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