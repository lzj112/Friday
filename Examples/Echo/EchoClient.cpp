
#include <stdio.h>

#include <iostream>

#include "src/Connector.h"
#include "src/EpollEventLoop.h"
using namespace std;

void newConn(int connfd) 
{
    printf("新的连接已经建立,new connfd = %d\n", connfd);    
}
int main() 
{
    // EpollEventLoop loop;
    SocketTCP sock;
    // sock.setNonBlocking();
    // Connector connector_(&loop, 
    //                      &sock,
    //                      "127.0.0.1",
    //                      4000);
    // connector_.setconnSucc(newConn);
    // connector_.connect();
    // loop.loop();
    InitSockAddr peer("127.0.0.1", 4000);
    sock.connect(peer);
    PackageTCP pack;
    strcpy(pack.body, "hello!");
    pack.head.length = 7;

    char ctr[20];
    int ret = 0;
    while (1) 
    {
        ret = send(sock.fd(), &pack, sizeof(pack), 0);
        printf("ret == %d\n", ret);
        ret = recv(sock.fd(), ctr, 20, 0);
        printf("ret-->%d, recv=%s\n", ret, ctr);
        sleep(1);
    }
}
