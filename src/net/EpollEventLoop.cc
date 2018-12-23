

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

    }
}

void EpollEventLoop::stopLoop() 
{

}

// void EpollEventLoop::updateEvents() 
// {

// }

void EpollEventLoop::updateTaskQueue(TaskQueue<int>& queue) 
{

}

void EpollEventLoop::updateTaskQueue(TaskQueue<IOcallBack&>& queue) 
{

}