/*
测试接受连接
*/

#include <../src/net/Server.h>

int mian() 
{
    EpollEventLoop loop;
    Server ser(&loop, "test", "127.0.0.1", 4000);
    ser.starts();
    loop.loop();
}