#ifndef THREADPOOL_H_
#define THREADPOOL_H_

#include <vector>

#include "../base/Thread.h"

class ThreadPool 
{
public:

private:
    std::vector<Thread> pool_;
};

#endif