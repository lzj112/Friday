#pragma once

#include <list>
#include <thread>
#include <memory>
#include <atomic>
#include <functional>

#include "FTaskQueue.h"


class FThreadPool 
{
public: 
    using Ftask = std::function<void()>;

    FThreadPool(int numThreads = 4);
    ~FThreadPool();
    
    void stop();
    void addTask(const Ftask& task);
    void addTask(Ftask&& task);

private:
    void startPool(int numThreads);
    void threadFunc();
    void stopThreadPool();

private:
    std::list<std::shared_ptr<std::thread> > Fpool_;
    FTaskQueue<Ftask> Fqueue_;
    std::atomic_bool isRunning;
    std::once_flag pollFlag;
};
