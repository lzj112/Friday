#include <cstdio>
#include <errno.h>
#include <cstdlib>

#include "EpollBase.h"

void EpollBase::add(int fd, epoll_event* ev) 
{
    if (epoll_ctl(epollFd, EPOLL_CTL_ADD, fd, ev) < 0) 
    {
        perror("epoll_ctl_add ");
        //错误处理 TODO
    }
}

void EpollBase::del(int fd, epoll_event* ev) 
{
    if (epoll_ctl(epollFd, EPOLL_CTL_DEL, fd, ev) < 0) 
    {
        perror("epoll_ctl_del ");
        //错误处理 TODO
    }
}

void EpollBase::ctl(int fd, epoll_event* ev) 
{
    if (epoll_ctl(epollFd, EPOLL_CTL_MOD, fd, ev) < 0) 
    {
        perror("epoll_ctl_mod ");
        //错误处理 TODO
    }
}

void EpollBase::wait(std::vector<epoll_event>& readyEvents, int timeout) 
{
    int eventsNumber = epoll_wait(epollFd,
                                  readyEvents.data(),
                                  static_cast<int> (readyEvents.size()),
                                  timeout);
    //有就绪事件
    if (eventsNumber > 0) 
    {
        //返回的事件数量等于容器容量,扩大容量
        if (eventsNumber == readyEvents.size()) 
        {
            readyEvents.resize(eventsNumber * 2);
        }
    }
    else if (eventsNumber == 0)  //超时返回无事件发生
    {
        //log TODO
        return ;
    }
    else 
    {
        if (errno == EINTR) //被信号中断
        {
            return ;
        }
        if (errno == EBADF ||   /*epfd不是一个有效文件描述符*/
            errno == EFAULT ||  /*对于events没有写全权限*/
            errno == EINVAL)    /*epfd不是一个epoll文件描述符或maxevnts<=0*/
        {
            //错误处理 TODO
            exit(1);    //退出 FIXME
        }
    }
}