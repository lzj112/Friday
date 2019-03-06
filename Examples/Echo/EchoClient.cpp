
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
    EpollEventLoop loop;
    SocketTCP sock;
    sock.setNonBlocking();
    sock.setNonBlocking();
    Connector connector_(&loop, 
                         &sock,
                         "127.0.0.1",
                         4000);
    connector_.setconnSucc(newConn);
    connector_.connect();
    loop.loop();
    printf("hello\n");
    InitSockAddr peer("127.0.0.1", 4000);
    sock.connect(peer);
    PackageTCP pack;
    strcpy(pack.body, "hello!");
    pack.head.length = 7;

    PackageTCP rePack;
    int ret = 0;
    while (1) 
    {
        ret = send(sock.fd(), &pack, sizeof(pack), 0);
        // assert(ret != -1);
        printf("send ret == %d\n", ret);
        ret = recv(sock.fd(), &rePack, PACKHEADSIZE, 0);
        // assert(ret != -1);
        ret = recv(sock.fd(), rePack.body, MAXBODY, 0);
        // assert(ret != -1);
        printf("recv = %s\n", rePack.body);
        usleep(1);
    }
}
