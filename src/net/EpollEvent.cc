

#include "../net/EpollEvent.h"


EpollEvent::EpollEvent()
{
    
}

void EpollEvent::updateReadEvents(int fd, epoll_event ev) 
{
    epollBase_->add(fd, &ev);
}

void EpollEvent::updateWriteEvents(int fd) 
{

}

void EpollEvent::delEvents() 
{

}

void EpollEvent::poll(std::vector<epoll_event>& events, int timeout) 
{
    epollBase_->wait(events, timeout);
}