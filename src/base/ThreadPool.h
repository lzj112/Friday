#ifndef THREADPOOL_H_
#define THREADPOOL_H_

#include <vector>

#include "../base/Thread.h"
#include "../net/EpollEventLoop.h"

class ThreadPool 
{
public:
    EpollEventLoop* getNextLoop();
private:
    std::vector<Thread> pool_;
};

#endif