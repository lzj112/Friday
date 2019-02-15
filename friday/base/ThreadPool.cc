
#include <assert.h> 

#include "../base/ThreadPool.h"


ThreadPool::ThreadPool() 
    : threadNums(4),
      nextThread(0),
      isCreated(false)
{

}

ThreadPool::~ThreadPool() 
{
}

void ThreadPool::start() 
{
    assert(!isCreated);
    for (int i = 0; i < threadNums; i++) 
    {
        //创建线程对象
        Thread* thread_ = new Thread();
        //保存线程对象
        pool_.push_back(std::unique_ptr<Thread> (thread_));
        thread_->startLoop();
        //保存创建的线程中的EpollEventLoop
        loops_.push_back(thread_->getLoop());

    }
}

EpollEventLoop* ThreadPool::getNextLoop() 
{
    EpollEventLoop* loop = loops_[nextThread];
    nextThread = (nextThread + 1) % threadNums;
    return loop;
}


