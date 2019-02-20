#ifndef EPOLLBASE_H_
#define EPOLLBASE_H_

#include <unistd.h>
#include <sys/epoll.h>

#include <vector>
#include <functional>


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

//封装epoll基础API
//不将析构函数声明为虚函数是因为
//不会使用该类指针指向子类对象
class EpollBase 
{
public:
    EpollBase() { epollFd = ::epoll_create(1); }
    EpollBase(const EpollBase&) = delete;
    EpollBase& operator=(const EpollBase&) = delete;
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