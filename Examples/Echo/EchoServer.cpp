
#include <assert.h>

#include <string>
#include <iostream>

#include "src/FServer.h"

using namespace std;

static int i = 0;

void echoFunc(MyEvent* ev, Message& m) 
{
    if (ev != nullptr)
    {
        const int length = m.length();
        char ctr[length + 10];
        strcpy(ctr, "ECHO : ");
        strcat(ctr, m.mes());
        Message tmp(ctr);
        ev->sendMes(tmp);
    }
}

int main() 
{
    EpollEventLoop loop;
    FServer ser(&loop, "test", "127.0.0.1", 4000);
    ser.setMesMgr(echoFunc);
    ser.starts();
    loop.loop();

    cout << "echo server 程序结束" << endl;    
}