
#include <utility>
#include <assert.h>

#include "EpollEventLoop.h"


const int EPOLLWAITFOR = 10000;

EpollEventLoop::EpollEventLoop() 
    : isLooping(false),
      isEnd(false),
      events(initEventSize),
      threadID (std::this_thread::get_id())
{
}

EpollEventLoop::~EpollEventLoop() 
{
    assert(isEnd);

    //关闭所有fd 
    removeAllEvents();
}

void EpollEventLoop::loop() 
{
    isLooping = true;
    while (isLooping) 
    {
        epoll_.wait(events, EPOLLWAITFOR);
        handleEvents();
        {
            // events.clear();
            std::vector<epoll_event> evs(events.size());
            std::swap(events, evs);
        }
    }

    isEnd = true;
}

void EpollEventLoop::handleEvents() 
{
    if (events.size() == 0) 
    {
        return ;
    }

    for (auto& x : events) 
    {
        MyEvent* ev = static_cast<MyEvent *> (x.data.ptr);
        
        if (x.events & pollRDHangUp) 
        {
            //设置可读,交给可读回调,触发其中错误处理
            printf("pollRDHangUp\n");
            delEvent(ev->fd());
            // x.events = pollReadAble;
        }
        if (x.events & pollReadAble)
        {
            printf("pollread\n");
            //有数据读到读buffer里
            ev->goRead();
            
        }
        if (x.events & pollWriteAble) 
        {
            printf("pollwrite\n");
            //有数据写到写buffer里
            ev->goWrite();
        }
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
            ::close(x.first);
        }
    }
}

void EpollEventLoop::delEvent(int fd) 
{
    auto it = eventsMap.find(fd);
    if (it != eventsMap.end()) 
        eventsMap.erase(it);
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
        epoll_.add(socket.fd(), &ev);  //将监听套接字加入epoll事件合集
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
        epoll_.add(socket.fd(), &ev);  //将监听套接字加入epoll事件合集
    }
}

void EpollEventLoop::modifyEvent(int type, MyEvent evT) 
{
    int sockfd = evT.fd();
    epoll_event ev;
    ev.events = type;
    auto it = eventsMap.find(sockfd);
    if (it != eventsMap.end())
        eventsMap.erase(it);

    eventsMap.insert(std::make_pair(sockfd, evT));
    
    auto iter = eventsMap.find(sockfd);
    if (iter != eventsMap.end()) 
    {
        ev.data.ptr = &((*iter).second);
        epoll_.ctl(sockfd, &ev);
    }
    else 
        printf("更改epoll注册事件失败\n");
}
