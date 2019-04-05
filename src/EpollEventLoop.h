#ifndef EPOLLEVENTLOOP_H_
#define EPOLLEVENTLOOP_H_

#include <memory>
#include <thread>
#include <unordered_map>

#include "UniqueID.h"
#include "MyEvent.h"
#include "FileDes.h"
#include "SocketTCP.h"
#include "EpollBase.h"

class MyEvent;

typedef std::map<int, std::shared_ptr<MyEvent> > EventMap;

//EventLoop 事件循环
class EpollEventLoop 
{
public:
    EpollEventLoop();
    ~EpollEventLoop();

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

    static const int initEventSize = 16;
    bool isLooping;
    bool isEnd;
    std::thread::id threadID;

    EpollBase epoll_;
    std::vector<epoll_event> events;    //存放epoll_wait返回结果

    EventMap eventsMap;
    std::vector<int> overdueEvent;

};

#endif
