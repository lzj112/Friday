#ifndef THREAD_H_
#define THREAD_H_

#include <thread>
#include <vector>
#include <memory>
#include <functional>

#include "Mutex.h"
#include "FileDes.h"
#include "TimerWheel.h"
#include "EpollEventLoop.h"


class Thread 
{
public:
    typedef std::function<void(void)> threadFunc;
    
    explicit Thread(const threadFunc& cb = nullptr);
    explicit Thread(const Thread&);
    ~Thread();
    Thread& operator=(const Thread&);

    void defaultWorkerThread();
    void Detach() { thread_.detach(); }
    void Join() { thread_.join(); }
    std::thread::id id() { return threadID; }
    void setThreadFunc(threadFunc& cb);
    
    void startLoop();
    EpollEventLoop* getLoop() { return loop_; }
 
private:
    std::thread thread_;
    std::thread::id threadID;
    bool isSetted;

    EpollEventLoop* loop_;
    std::unique_ptr<Mutex> myMutex;
<<<<<<< HEAD
    // TimerWheel wheel;
=======
    TimerWheel wheel;
>>>>>>> 2dbba237396801c4e1d576a40693b98ce9c368c1
    // std::vector<epoll_event> readyEvents;
};
#endif
