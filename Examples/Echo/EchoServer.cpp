/*
测试接受连接
*/

// #include "/home/lzj/Friday/friday/net/FServer.h"
#include "friday/net/FServer.h"

int mian() 
{
    EpollEventLoop loop;
    FServer ser(&loop, "test", "127.0.0.1", 4000);
    ser.starts();
    loop.loop();
}