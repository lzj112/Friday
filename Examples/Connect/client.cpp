
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
}
