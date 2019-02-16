/*
测试接受连接
*/

#include "src/FServer.h"

int main() 
{
    EpollEventLoop loop;
    FServer ser(&loop, "test", "127.0.0.1", 4000);
    ser.starts();
    loop.loop();
}