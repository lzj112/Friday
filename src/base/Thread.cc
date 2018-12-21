
#include "../base/Thread.h" 

Thread::Thread(FileDes& fd, threadFunc& cb) :
    threadFunc_(cb),
    threadID(-1)
{
    std::thread tmp(cb, fd);
    t = std::move(tmp);
    setID();
}

void Thread::setID() 
{
    std::thread::id id = t.get_id();
}