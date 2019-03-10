
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

    PackageTCP pack(123, "hello world!");

    PackageTCP retPack;
    ret = 0;
    int sendCount = 0;
    int recvCount = 0;
    signal(SIGALRM, timerFunc);
    // alarm(180);
    
    alarm(3);

    while (echo) 
    {
        memset(&retPack, 0, sizeof(retPack));
        ret = send(sock, &pack, sizeof(PackageTCP), 0);
        sendCount += ret;
        printf("Send = %s\n",pack.body);
        ret = recv(sock, &retPack, PACKHEADSIZE, 0);
        recvCount += ret;
        ret = recv(sock, retPack.body, MAXBODY, 0);
        recvCount += ret;
        printf("recv = %s\n", retPack.body);
    }
    printf("send = %d bytes recv = %dbytes in 3 mins\n", sendCount, recvCount);
}
