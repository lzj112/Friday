#ifndef EPOLLEVENTLOOP_H_
#define EPOLLEVENTLOOP_H_

#include <memory>
#include <thread>

#include "../net/EpollEvent.h"

class EpollEventLoop 
{
public:
    EpollEventLoop();
    ~EpollEventLoop();

    void loop();

    void stopLoop();

    void updateEvents();
    
private:
    bool isLooping;

    std::unique_ptr<EpollEvent> poll_;
    std::thread::id threadID;

};

#endif

/*
添加任务队列
*/