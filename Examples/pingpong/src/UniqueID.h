#ifndef UNIQUEID_H_
#define UNIQUEID_H_

#include <stdint.h>
#include <assert.h>
#include <sys/timerfd.h>

#define IDSPOCE 100000

class UniqueID 
{
public:
    UniqueID() 
    {}
    static int ID() 
    {
        timespec nowTime;
        int ret = clock_gettime(CLOCK_REALTIME, &nowTime);
        assert(ret != -1);
        return static_cast<int> (nowTime.tv_nsec);
    }
};

#endif