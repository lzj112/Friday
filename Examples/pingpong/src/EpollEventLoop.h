#ifndef EPOLLEVENTLOOP_H_
#define EPOLLEVENTLOOP_H_

#include <memory>
#include <thread>

#include "MyEvent.h"
#include "FileDes.h"
#include "SocketTCP.h"
#include "EpollBase.h"

class MyEvent;

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
   
    
private:
    bool isLooping;
    bool isEnd;
    std::thread::id threadID;

    EpollBase epoll_;
    std::vector<epoll_event> events;    //存放epoll_wait返回结果
    // std::map<int, MyEvent> eventsMap;   //存放fd和ptr指向结构体的对应关系
    std::map<int, std::shared_ptr<MyEvent> > eventsMap;

    static const int initEventSize = 16;
};

#endif
