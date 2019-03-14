

#include <string>
#include <iostream>

#include "src/ErrLog.h"
#include "src/FServer.h"

using namespace std;

void pingpongFunc(MyEvent* ev, Message& m) 
{
    DEBUG("pingpong===%s///\n", m.mes());
    ev->sendMes(m);
}

int main() 
{
    EpollEventLoop loop;
    FServer ser(&loop, "test", "127.0.0.1", 6000);
    ser.setMesMgr(pingpongFunc);
    ser.starts();
    loop.loop();

    cout << "echo server 程序结束" << endl;    
}
