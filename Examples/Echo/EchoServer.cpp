
<<<<<<< HEAD
#include <assert.h>

=======
>>>>>>> 2dbba237396801c4e1d576a40693b98ce9c368c1
#include <string>
#include <iostream>

#include "src/FServer.h"

using namespace std;

<<<<<<< HEAD
static int i = 0;

void echoFunc(MyEvent* ev, Message& m) 
{
    assert(ev != nullptr);
    const int length = m.length();
    char ctr[length + 10];
    strcpy(ctr, "ECHO : ");
    strcat(ctr, m.mes());
    Message tmp(ctr);
    ev->sendMes(tmp);
=======
void echoFunc(MyEvent* ev, Message& m) 
{
    const int length = m.length();
    char ctr[length + 10];
    strcpy(ctr, "ECHO : ");
    strcat(ctr, m.mess());
    Message tmp(ctr);
    ev->sendMess(tmp);
>>>>>>> 2dbba237396801c4e1d576a40693b98ce9c368c1
}

int main() 
{
    EpollEventLoop loop;
    FServer ser(&loop, "test", "127.0.0.1", 4000);
<<<<<<< HEAD
    ser.setMesMgr(echoFunc);
=======
    ser.setMesMang(echoFunc);
>>>>>>> 2dbba237396801c4e1d576a40693b98ce9c368c1
    ser.starts();
    loop.loop();

    cout << "echo server 程序结束" << endl;    
}