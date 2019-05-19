
#pragma once

#include <vector>

#include "IOThread.h"
#include "EpollEventLoop.h"

class ThreadPool 
{
public:
    ThreadPool(int threadnum);
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
