
#include <utility>
#include <assert.h>

#include <memory>

#include "ErrLog.h"
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
    }

    isEnd = true;
}

void EpollEventLoop::handleEvents() 
{
    for (auto& x : events) 
    {
        MyEvent* ev = static_cast<MyEvent *> (x.data.ptr);
        
        if (x.events & pollRDHangUp ) 
        {
            //设置可读,交给可读回调,触发其中错误处理
            x.events = pollReadAble;
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
        if (!overdueEvent.empty())
            disposalWaste();
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

void EpollEventLoop::dumpster(int eventId) 
{
    overdueEvent.emplace_back(eventId);
}

void EpollEventLoop::disposalWaste() 
{
    if (!overdueEvent.empty()) 
        for (auto x : overdueEvent)
        {
            eventsMap.erase(x);
        }
}

void EpollEventLoop::delEvent(int fd) 
{
    DEBUG("删除%d对应事件\n", fd);
    auto it = eventsMap.find(fd);
    if (it != eventsMap.end()) 
        eventsMap.erase(it);
}


void EpollEventLoop::regReadable(std::shared_ptr<MyEvent> myEv) 
{
    epoll_event ev = {0, {0}};
    ev.events = pollReadAble | pollEdgeTrigger;

    ev.data.ptr = myEv.get();
    if (epoll_.add(myEv->fd(), &ev) != -1) 
    {
        eventsMap.insert(std::make_pair(myEv->fd(), myEv));
    }
}

void EpollEventLoop::regWriteable(std::shared_ptr<MyEvent> myEv) 
{
    epoll_event ev = {0, {0}};
    ev.events = pollWriteAble | pollEdgeTrigger;

    ev.data.ptr = myEv.get();
    if (epoll_.add(myEv->fd(), &ev) != -1) 
    {
        eventsMap.insert(std::make_pair(myEv->fd(), myEv));
    }
}

void EpollEventLoop::modifyEvent(int type, std::shared_ptr<MyEvent> myEv) 
{
    int sockfd = myEv->fd();
    epoll_event ev;
    ev.events = type;
    ev.data.ptr = myEv.get();
    if (epoll_.ctl(myEv->fd(), &ev) == -1) 
    {
        DEBUG("更改 epoll 事件类型失败\n");
        auto it = eventsMap.find(myEv->fd());
        if (it != eventsMap.end()) 
            eventsMap.erase(it);
    }
}
