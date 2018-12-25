#ifndef EPOLLEVENTLOOP_H_
#define EPOLLEVENTLOOP_H_

#include <memory>
#include <thread>

#include "../net/MyEvent.h"
#include "../base/FileDes.h"
#include "../net/SocketTCP.h"
#include "../net/EpollEvent.h"
#include "../base/TaskQueue.h"


//EventLoop 事件循环

class EpollEventLoop 
{
public:
    EpollEventLoop();
    ~EpollEventLoop();

    void loop();

    void stopLoop();
    void removeAllEvents();

    void regReadable(int fd);
    void regReadable(MyEvent socket);
    void regWriteable(MyEvent socket);
   
    void updateTaskQueue(TaskQueue<int>& queue);
    void updateTaskQueue(TaskQueue<IOcallBack&>& queue);
    
private:
    bool isLooping;
    bool isEnd;
    std::thread::id threadID;

    std::unique_ptr<EpollEvent> epoll_;
    std::vector<epoll_event> events;    //存放epoll_wait返回结果
    std::map<int, MyEvent> eventsMap;   //存放fd和ptr指向结构体的对应关系

};

#endif

/*
添加任务队列
*/