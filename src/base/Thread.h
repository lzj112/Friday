#ifndef THREAD_H_
#define THREAD_H_

#include <thread>
#include <vector>
#include <memory>
#include <functional>

#include "../base/FileDes.h"
#include "../base/TaskQueue.h"
#include "../net/EpollEventLoop.h"

class Thread 
{
public:
    typedef std::function<void()> threadFunc;

    explicit Thread(FileDes fd);
    explicit Thread(FileDes fd, threadFunc& cb);

    void defaultThreadFunc();
    void Detach() { thread_.detach(); }
    void setID();
    void setThreadFunc(threadFunc& cb) { threadFunc_ = cb; }
    void updateTaskQueue();

private:
    std::thread thread_;
    threadFunc threadFunc_;
    std::thread::id threadID;

    EpollEventLoop* loop_;
    std::unique_ptr<TaskQueue<IOcallBack&> > taskQueue;
};
#endif
