
#include <assert.h>

#include <string>
#include <vector>
#include <iostream>

#include "src/ErrLog.h"
#include "src/FServer.h"

using namespace std;

void echoFunc(MyEvent* ev, vector<unsigned char> str) 
{
    vector<unsigned char> tmpBuf;
    auto it = str.begin();
    auto end = str.end();
    for (; it != end; it++) 
    {
        if (*it != '\0') 
            tmpBuf.push_back(*it);
        else 
        {
            tmpBuf.push_back(*it);
            ev->sendMsgToBuffer(tmpBuf);
            tmpBuf.clear();
        }
    }
}

int main() 
{
    EpollEventLoop loop;
    FServer ser(&loop, "test", "127.0.0.1", 4000);
    ser.setMsgMgr(echoFunc);
    ser.starts();
    loop.loop();

    cout << "echo server 程序结束" << endl;    
}