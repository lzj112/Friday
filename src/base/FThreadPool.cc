
#include "FThreadPool.h"

FThreadPool::FThreadPool(int numThreads) 
{ startPool(numThreads); }

FThreadPool::~FThreadPool()
{
    try 
    {
        std::call_once(pollFlag, 
                   [this]{
                       stopThreadPool();
                   });
    }
    catch (...) 
    {}
}

void FThreadPool::stop() 
{   
    std::call_once(pollFlag, 
                   [this]{
                       stopThreadPool();
                   });
}

void FThreadPool::addTask(const Ftask& task) 
{
    Fqueue.putTask(task);
}

void FThreadPool::addTask(Ftask&& task) 
{
    Fqueue.putTask(std::forward<Ftask> (task));
}

void FThreadPool::startPool(int numThreads) 
{
    isRunning = true;
    for (int i = 0; i < numThreads; i++) 
    {
        Fpool.emplace_back(std::make_shared<std::thread> 
            (&FThreadPool::threadFunc, this));
    }
}

void FThreadPool::threadFunc() 
{
    while (isRunning) 
    {
        std::list<Ftask> taskList;
        Fqueue.takeTask(taskList);

        for (auto& task : taskList) 
        {
            if (!isRunning) 
                return;
            
            task();
        }
    }
}

void FThreadPool::stopThreadPool() 
{
    Fqueue.stop();
    isRunning = false;

    for (auto thread : Fpool) 
    {
        if (thread) 
            thread->join();
    }

    Fpool.clear();
}
