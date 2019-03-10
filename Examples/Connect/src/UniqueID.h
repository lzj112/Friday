#ifndef UNIQUEID_H_
#define UNIQUEID_H_

#include <stdint.h>
#include <assert.h>
#include <sys/timerfd.h>

class UniqueID 
{
public:
    UniqueID() 
    {
        int ret = clock_gettime(CLOCK_REALTIME, &nowTime);
        assert(ret != -1);
    }
    uint32_t ID() 
    {
        return static_cast<uint32_t> (nowTime.tv_nsec);
    }
private:
    timespec nowTime;
};

#endif