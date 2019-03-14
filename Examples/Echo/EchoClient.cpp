
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

#include <iostream>

#include "src/Timer.h"
#include "src/Connector.h"
#include "src/EpollEventLoop.h"
using namespace std;

static bool echo = true;

void newConn(int connfd) 
{
    printf("新的连接已经建立,new connfd = %d\n", connfd);    
}

void timerFunc(int) 
{
    echo = false;
}

int main() 
{

    InitSockAddr serAddr("127.0.0.1", 4000);
   
    int sock = ::socket(AF_INET, SOCK_STREAM, 0);
    assert(sock != -1);

    int ret = ::connect(sock, 
                        serAddr.sockAddr(), 
                        serAddr.length());
    assert(ret != -1);
    unsigned char tmpBuf[20] = "Hello Wold";
    int len = 11;

    unsigned char recvBuf[20];
    while (1) 
    {
        ::send(sock, tmpBuf, len, 0);
        ::recv(sock, recvBuf, sizeof(recvBuf), 0);
        cout << "recv = " << recvBuf << endl;
    }


    // ret = 0;
    // int sendCount = 0;
    // int recvCount = 0;
    // signal(SIGALRM, timerFunc);
    // alarm(600);

    


    // printf("send = %d bytes recv = %dbytes in 3 mins\n", sendCount, recvCount);
}
