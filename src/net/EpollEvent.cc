
#include <assert.h>

#include "../net/EpollEvent.h"


EpollEvent::EpollEvent()
{
    
}

EpollEvent::~EpollEvent() 
{
    assert(!ishandling);
}

void EpollEvent::updateReadEvents(int fd, epoll_event ev) 
{
    epollBase_->add(fd, &ev);
}

void EpollEvent::updateWriteEvents(int fd, epoll_event ev) 
{
    epollBase_->add(fd, &ev);
}

void EpollEvent::delEvents(int fd, epoll_event* ev) 
{
    if (ev == nullptr) 
    {
        //log TODO
        return ;
    }
    epollBase_->del(fd, ev);
}

void EpollEvent::poll(std::vector<epoll_event>& events, int timeout) 
{
    epollBase_->wait(events, timeout);
}

void EpollEvent::handleEvent(std::vector<epoll_event>& events) 
{
    if (events.size() == 0) 
    {
        //log TODO
        return ;
    }

    for (auto& x : events) 
    {
        MyEvent* ev = static_cast<MyEvent *> (x.data.ptr);
        
        if (x.events & (pollHangUp | pollErr)) 
        {
            //这两个事件发生,设置可读
            //交给可读回调,触发其中错误处理
            //尝试重连?
            x.events = pollReadAble;
        }

        if (x.events & pollReadAble)
        {
            if (x.events & pollRDHangUp) 
            {
                //对端正常关闭,同时触发epollin
                ::close(ev->fd());
            }
            else 
                //有数据读到读buffer里
                ev->readCallBack_(ev->fd());
        }

        if (x.events & pollWriteAble) 
        {
            //有数据写到写buffer里
            ev->writeCallBack_(ev->fd());
        }                      
       

    }
}