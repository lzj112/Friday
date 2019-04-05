
#include <stdint.h>
#include <assert.h>
#include <sys/timerfd.h>

#include <iostream>
using namespace std;

#define IDSPOCE 100000
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
        return static_cast<uint32_t> (nowTime.tv_nsec) % IDSPOCE;
    }
private:
    timespec nowTime;
};

int main() 
{
    UniqueID a;
    cout << a.ID() << endl;
}