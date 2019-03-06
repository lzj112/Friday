
#include <string>
#include <iostream>

#include "src/FServer.h"

using namespace std;

int main() 
{
    EpollEventLoop loop;
    FServer ser(&loop, "test", "127.0.0.1", 4000);
    ser.starts();
    loop.loop();

    cout << "echo server 程序结束" << endl;    
}