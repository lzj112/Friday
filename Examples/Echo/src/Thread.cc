
#include <assert.h>

#include "Thread.h" 

Thread::Thread() 
    : loop_(new EpollEventLoop()),
      threadFunc_(std::bind(&Thread::defaultThreadFunc, 
                            this)),
      threadID(-1), 
      isSetted(false),
      myMutex(new Mutex())
{
    std::thread tmp(threadFunc_);
    thread_ = std::move(tmp);

    isSetted = true;
}

Thread::~Thread() 
{
    loop_->stopLoop();
    Join();

    delete loop_;
}

void Thread::setID() 
{
    assert(isSetted);

    std::thread::id id = thread_.get_id();
}

void Thread::setThreadFunc(threadFunc& cb) 
{
    isSetted = false;
    std::thread tmp(cb);
    thread_ = std::move(tmp);
    isSetted = true;

    setID();
}

void Thread::defaultThreadFunc() 
{

}

void Thread::startLoop() 
{
    loop_->loop();
}