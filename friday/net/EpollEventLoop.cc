
#include <utility>
#include <assert.h>

#include "../net/EpollEventLoop.h"


const int EPOLLWAITFOR = 10000;

EpollEventLoop::EpollEventLoop() 
    : isLooping(false),
      isEnd(false),
      epoll_(new EpollBase()),
      threadID (std::this_thread::get_id())
{

}

EpollEventLoop::~EpollEventLoop() 
{
    assert(!isEnd);

    //关闭所有fd 
    removeAllEvents();
}

void EpollEventLoop::loop() 
{
    isLooping = true;
    while (isLooping) 
    {
        events.clear();
        epoll_->wait(events, EPOLLWAITFOR);

        handleEvents();
    }

    isEnd = true;
}


void EpollEventLoop::handleEvents() 
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
            //这两个事件发生,设置可读
            //交给可读回调,触发其中错误处理
            x.events = pollReadAble;

        if (x.events & pollReadAble)
            if (x.events & pollRDHangUp) 
                //对端正常关闭,同时触发epollin
                ::close(ev->fd());
            else 
                //有数据读到读buffer里
                ev->goRead();

        if (x.events & pollWriteAble) 
            //有数据写到写buffer里
            ev->goWrite();
    }
}

void EpollEventLoop::stopLoop() 
{
    isLooping = false;
}

void EpollEventLoop::removeAllEvents() 
{
    if (isEnd || !isLooping) 
    {
        for (auto x : eventsMap) 
        {
            epoll_event ev;
            ev.data.ptr = static_cast<void *> (&x.second);
            delEvent(x.first);
            ::close(x.first);
        }
    }
}

void EpollEventLoop::delEvent(int fd) 
{
    epoll_event ev;
    ev.data.fd = fd;
    epoll_->del(fd, &ev);
    eventsMap.erase(fd);
}


void EpollEventLoop::regReadable(int fd) 
{
    MyEvent tmp(this, fd); 
    regReadable(tmp);
}

void EpollEventLoop::regReadable(MyEvent socket) 
{
    epoll_event ev = {0, {0}};
    ev.events = pollReadAble | pollEdgeTrigger;
    eventsMap.insert(std::make_pair(socket.fd(), socket));
  
    auto it = eventsMap.find(socket.fd());
    if (it != eventsMap.end()) 
    {
        ev.data.ptr = &((*it).second);
        epoll_->add(socket.fd(), &ev);  //将监听套接字加入epoll事件合集
    }
}

void EpollEventLoop::regWriteable(MyEvent socket) 
{
    epoll_event ev = {0, {0}};
    ev.events = pollWriteAble | pollEdgeTrigger;
    eventsMap.insert(std::make_pair(socket.fd(), socket));
  
    auto it = eventsMap.find(socket.fd());
    if (it != eventsMap.end()) 
    {
        ev.data.ptr = &((*it).second);
        epoll_->add(socket.fd(), &ev);  //将监听套接字加入epoll事件合集
    }
}

void EpollEventLoop::modifyEvent(epoll_event ev) 
{
    MyEvent* tmp = static_cast<MyEvent *> (ev.data.ptr);
    int sockfd = tmp->fd();
    auto it = eventsMap.find(sockfd);
    if (it != eventsMap.end())
        eventsMap.erase(it);
    eventsMap.insert(std::make_pair(sockfd, *tmp));
    
    auto iter = eventsMap.find(sockfd);
    if (iter != eventsMap.end()) 
    {
        ev.data.ptr = &((*iter).second);
        epoll_->ctl(sockfd, &ev);
    }
}
