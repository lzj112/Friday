#ifndef THREAD_H_
#define THREAD_H_

#include <thread>
#include <functional>

#include "../net/FileDes.h"

class Thread 
{
public:
    typedef std::function<void(FileDes&)> threadFunc;

    explicit Thread(FileDes& fd, threadFunc& cb);

    void Detach() { t.detach(); }
    void setID();
    void setThreadFunc(threadFunc& cb) { threadFunc_ = cb; }

private:
    
    threadFunc threadFunc_;

    std::thread::id threadID;
    std::thread t;
};
#endif