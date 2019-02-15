#ifndef THREADPOOL_H_
#define THREADPOOL_H_

#include <vector>

#include "../base/Thread.h"
#include "../net/EpollEventLoop.h"

class ThreadPool 
{
public:
    ThreadPool();
    ~ThreadPool();

    void start();

    void setThreadNums(int nums) { threadNums = nums; }
    EpollEventLoop* getNextLoop();
private:
    std::vector<std::unique_ptr<Thread> > pool_;
    std::vector<EpollEventLoop *> loops_;

    int threadNums;
    int nextThread;
    bool isCreated;

};

#endif