
#include <iostream>

#include "src/FServer.h"

using namespace std;

void echoFunc(MyEvent* ev, Message& m) 
{
    const int length = m.length();
    char ctr[length + 10];
    strcpy(ctr, "ECHO : ");
    strcat(ctr, m.mess());
    Message tmp(ctr);
    ev->sendMess(tmp);
}

int main() 
{
    EpollEventLoop loop;
    FServer ser(&loop, "test", "127.0.0.1", 4000);
    ser.setMesMang(echoFunc);
    ser.starts();
    loop.loop();

    cout << "echo server 程序结束" << endl;    
}