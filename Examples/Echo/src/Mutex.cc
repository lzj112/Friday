
#include "Mutex.h"

Mutex::~Mutex() 
{
    if (isLock) 
    {
        myLock.unlock();
        myLock.~mutex();
    }
    else 
        myLock.~mutex();
}

void Mutex::locked() 
{
    if (isLock == false)
    {
        myLock.lock(); 
        isLock = true;
    }
    else 
        return ;
}

void Mutex::free() 
{
    if (isLock == true) 
    {
        myLock.unlock();
        isLock = false;
    }
    else 
        return ;
}

bool Mutex::tryToLock() 
{
    if (myLock.try_lock()) 
    {
        isLock = true;
        return true;
    }
    else 
        return false;
}