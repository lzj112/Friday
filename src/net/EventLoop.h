#pragma once

#include <memory>
#include <thread>
#include <unordered_map>

#include "MyEvent.h"
#include "FileDes.h"
#include "UniqueID.h"
#include "SocketTCP.h"
#include "EpollBase.h"
#include "FThreadPool.h"

class MyEvent;

typedef std::map<int, std::shared_ptr<MyEvent> > EventMap;

//EventLoop 事件循环
class EventLoop 
{
public:
    EventLoop();
    ~EventLoop();

    void loop();
    void handleEvents();

    void stopLoop();
    void removeAllEvents();

    void delEvent(int fd);
    void regReadable(std::shared_ptr<MyEvent> myEv);
    void regWriteable(std::shared_ptr<MyEvent> myEv);
    void modifyEvent(int type, std::shared_ptr<MyEvent> myEv);
    void dumpster(int);
   
    
private:
    void disposalWaste();
    
private:
    static const int initEventSize = 16;
    static const int initThreads = 4;
    bool isLooping;
    bool isEnd;
    std::thread::id threadID;

    EpollBase epoll_;
    std::vector<epoll_event> events;    //存放epoll_wait返回结果

    EventMap eventsMap;
    std::vector<int> overdueEvent;  //过期的event

    FThreadPool pool;
};

