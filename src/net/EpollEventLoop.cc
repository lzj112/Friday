
#include <utility>

#include "../net/EpollEventLoop.h"


const int EPOLLWAITFOR = 10000;

EpollEventLoop::EpollEventLoop() 
    : isLooping(false),
      epoll_(new EpollEvent()),
      threadID (std::this_thread::get_id())
{

}


void EpollEventLoop::loop() 
{
    isLooping = true;
    while (isLooping) 
    {
        events.clear();
        epoll_->poll(events, EPOLLWAITFOR);

        epoll_->handleEvent();
    }
}

void EpollEventLoop::stopLoop() 
{

}

void EpollEventLoop::regReadable(MyEvent socket) 
{
    epoll_event ev = {0, {0}};
    ev.events = pollReadAble | pollEdgeTrigger;
    eventsMap.insert(std::make_pair(socket.fd(), socket));
  
    auto it = eventsMap.find(socket.fd());
    ev.data.ptr = &(*it).second;

    epoll_->updateReadEvents(socket.fd(), ev);  //将监听套接字加入epoll事件合集
}

void EpollEventLoop::updateTaskQueue(TaskQueue<int>& queue) 
{

}

void EpollEventLoop::updateTaskQueue(TaskQueue<IOcallBack&>& queue) 
{

}