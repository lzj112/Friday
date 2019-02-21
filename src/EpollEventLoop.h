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
    // void regReadable(int fd);
    void regReadable(MyEvent socket);
    // void regWriteable(int fd);
    void regWriteable(MyEvent socket);
    void modifyEvent(int type, MyEvent evT);
   
    
private:
    bool isLooping;
    bool isEnd;
    std::thread::id threadID;

    std::unique_ptr<EpollBase> epoll_;
    std::vector<epoll_event> events;    //存放epoll_wait返回结果
    std::map<int, MyEvent> eventsMap;   //存放fd和ptr指向结构体的对应关系

    static const int initEventSize = 16;
};

#endif
