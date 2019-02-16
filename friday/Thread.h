#ifndef THREAD_H_
#define THREAD_H_

#include <thread>
#include <vector>
#include <memory>
#include <functional>

#include "Mutex.h"
#include "FileDes.h"
#include "EpollEventLoop.h"

typedef std::function<void()> threadFunc;

class Thread 
{
public:

    explicit Thread();
    explicit Thread(const Thread&);
    ~Thread();
    Thread& operator=(const Thread&);

    void defaultThreadFunc();
    void Detach() { thread_.detach(); }
    void Join() { thread_.join(); }
    void setID();
    void setThreadFunc(threadFunc& cb);
    
    void startLoop();
    EpollEventLoop* getLoop() { return loop_; }
    // void updateTaskQueue();
 
private:
    std::thread thread_;
    threadFunc threadFunc_;
    std::thread::id threadID;
    bool isSetted;

    EpollEventLoop* loop_;
    std::unique_ptr<Mutex> myMutex;
    std::vector<epoll_event> readyEvents;
    // std::unique_ptr<TaskQueue<IOcallBack&> > taskQueue;
};
#endif
