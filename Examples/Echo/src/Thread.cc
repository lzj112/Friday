
#include <assert.h>

#include "Thread.h" 

Thread::Thread(const threadFunc& cb) 
    : loop_(new EpollEventLoop()),
      threadID(-1), 
      isSetted(false),
      myMutex(new Mutex())
{
    if (cb == nullptr) 
    {
        std::thread tmp(std::bind(&Thread::defaultWorkerThread, this));
        thread_ = std::move(tmp);
        std::thread::id id = thread_.get_id();
    }
    else 
    {
        std::thread tmp(cb);
        thread_ = std::move(tmp);
        std::thread::id id = thread_.get_id();
    }
}
 
Thread::~Thread() 
{
    loop_->stopLoop();
    Join();

    delete loop_;
}

void Thread::defaultWorkerThread() 
{
    loop_->loop();
}

