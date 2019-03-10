
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

    InitSockAddr serAddr("127.0.0.1", 4000);
   
    int sock = ::socket(AF_INET, SOCK_STREAM, 0);
    assert(sock != -1);

    int ret = ::connect(sock, 
                        serAddr.sockAddr(), 
                        serAddr.length());
    assert(ret != -1);

    PackageTCP pack(123, "FUCK!");

    PackageTCP retPack;
    ret = 0;
    int count = 0;
    while (1) 
    {
        memset(&retPack, 0, sizeof(retPack));
        ret = send(sock, &pack, sizeof(PackageTCP), 0);
        assert(ret != -1);
        printf("send ret == %d\n", ret);
        ret = recv(sock, &retPack, PACKHEADSIZE, 0);
        // assert(ret != -1);
        ret = recv(sock, retPack.body, MAXBODY, 0);
        // assert(ret != -1);
        printf("recv =%d  %s\n", ret, retPack.body);
        count += ret;
        // usleep(1);
    //     sleep(1);
    }
    while (1) {}
}
