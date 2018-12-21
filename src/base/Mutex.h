#ifndef MUTEX_H_
#define MUYEX_H_

#include <mutex>

class Mutex 
{
public:
    Mutex() : isLock(false) {}
    ~Mutex();

    Mutex(const Mutex& tmp) = delete;
    Mutex& operator=(const Mutex& p) = delete;

    void locked();
    void free();
    bool tryToLock();
    
private:
    bool isLock;
    std::mutex myLock;
};

#endif