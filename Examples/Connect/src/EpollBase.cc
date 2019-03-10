#include <cstdio>
#include <errno.h>
#include <cstdlib>
#include <stdio.h>

#include <iostream>

#include "ErrLog.h"
#include "MyEvent.h"
#include "EpollBase.h"

void EpollBase::add(int fd, epoll_event* ev) 
{
    MyEvent* ptr = static_cast<MyEvent *> (ev->data.ptr);
    epoll_ctl(epollFd, EPOLL_CTL_ADD, fd, ev);
}

void EpollBase::del(int fd, epoll_event* ev) 
{
    epoll_ctl(epollFd, EPOLL_CTL_DEL, fd, ev);
}

void EpollBase::ctl(int fd, epoll_event* ev) 
{
    epoll_ctl(epollFd, EPOLL_CTL_MOD, fd, ev);
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
            perror("被信号中断:");
            DEBUG("epfd = %d size=%d, timeout=%d\n", epollFd, 
                                                     static_cast<int> (readyEvents.size()), 
                                                     timeout);
            exit(1);    //退出 FIXME
        }
    }
}