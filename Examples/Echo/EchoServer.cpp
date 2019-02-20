
#include <iostream>

#include "src/FServer.h"

using namespace std;

void echoFunc(MyEvent* ev, Message& m) 
{
    // PackageTCP package;
    // strcpy(package.body, m.mess());
    // package.head.type = m.type();
    // package.head.length = sizeof(package.body);

    ev->sendMess(m);
}

int main() 
{
    EpollEventLoop loop;
    FServer ser(&loop, "test", "127.0.0.1", 4000);
    ser.starts();
    loop.loop();

    cout << "echo server 程序结束" << endl;    
}