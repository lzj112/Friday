
#include "FThreaPool.h"

FThreadPool::FThreadPool(int numThreads) 
{   startPool(numThreads);  }

FThreadPool::~FThreadPool()
{
    std::call_once(pollFlag, 
                   [this]{
                       stopThreadPool();
                   });
}

void FThreaPool::stop() 
{   
    std::call_once(pollFlag, 
                   [this]{
                       stopThreadPool();
                   });
}

void FThreaPool::addTask(const Ftask& task) 
{
    Fqueue_.putTask(task);
}

void FThreaPool::addTask(Ftask&& task) 
{
    Fqueue.putTask(std::forward<Ftask> (task));
}

void FThreaPool::startPool(int numThreads) 
{
    isRunning = true;
    for (int i = 0; i < numThreads; i++) 
    {
        Fpool_.emplace_back(std::make_shared<std::thread> 
            (&FThreadPool::threadFunc, this));
    }
}

void FThreaPool::threadFunc() 
{
    while (isRunning) 
    {
        std::list<Ftask> taskList;
        Fqueue_.takeTask(taskList);

        for (auto& task : taskList) 
        {
            if (!isRunning) 
                return;
            
            task();
        }
    }
}

void FThreaPool::stopThreadPool() 
{
    Fqueue_.stop();
    isRunning = false;

    for (auto thread : Fpool_) 
    {
        if ()
    }
}
