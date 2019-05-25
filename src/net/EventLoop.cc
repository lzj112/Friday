
#include <utility>
#include <assert.h>

#include <memory>

#include "ErrLog.h"
#include "EventLoop.h"


const int EPOLLWAITFOR = 10000;

EventLoop::EventLoop() 
    : isLooping(false),
      isEnd(false),
      events(initEventSize),
      threadID (std::this_thread::get_id()),
      pool(initThreads)
{}

EventLoop::~EventLoop() 
{
    assert(isEnd);

    //关闭所有fd 
    removeAllEvents();
}

void EventLoop::loop() 
{
    isLooping = true;
    while (isLooping) 
    {
        epoll_.wait(events, EPOLLWAITFOR);
        handleEvents();
    }

    isEnd = true;
}

void EventLoop::handleEvents() 
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
            DEBUG("pollread\n");
            //有数据读到读buffer里
            pool.addTask(std::bind(MyEvent::goRead, ev));
        }
        if (x.events & pollWriteAble) 
        {
            DEBUG("pollwrite\n");
            //有数据写到写buffer里
            pool.addTask(std::bind(MyEvent::goWrite, ev));
        }
        if (!overdueEvent.empty())
            disposalWaste();
    }
}

void EventLoop::stopLoop() 
{
    isLooping = false;
}

void EventLoop::removeAllEvents() 
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

void EventLoop::dumpster(int eventId) 
{
    overdueEvent.emplace_back(eventId);
}

void EventLoop::disposalWaste() 
{
    if (!overdueEvent.empty()) 
        for (auto x : overdueEvent)
        {
            eventsMap.erase(x);
        }
}

void EventLoop::delEvent(int fd) 
{
    DEBUG("删除%d对应事件\n", fd);
    auto it = eventsMap.find(fd);
    if (it != eventsMap.end()) 
        eventsMap.erase(it);
}


void EventLoop::regReadable(std::shared_ptr<MyEvent> myEv) 
{
    epoll_event ev = {0, {0}};
    ev.events = pollReadAble | pollEdgeTrigger;

    ev.data.ptr = myEv.get();
    if (epoll_.add(myEv->fd(), &ev) != -1) 
    {
        eventsMap.insert(std::make_pair(myEv->fd(), myEv));
    }
}

void EventLoop::regWriteable(std::shared_ptr<MyEvent> myEv) 
{
    epoll_event ev = {0, {0}};
    ev.events = pollWriteAble | pollEdgeTrigger;

    ev.data.ptr = myEv.get();
    if (epoll_.add(myEv->fd(), &ev) != -1) 
    {
        eventsMap.insert(std::make_pair(myEv->fd(), myEv));
    }
}

void EventLoop::modifyEvent(int type, std::shared_ptr<MyEvent> myEv) 
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
