
#include <stdio.h>

#include <thread>
#include <vector>
#include <iostream>

#include "src/Connector.h"
#include "src/EpollEventLoop.h"
using namespace std;

int main() 
{
    InitSockAddr serAddr("127.0.0.1", 4000);
    
    pid_t t[3];
    for (int i = 0; i < 3; i++) 
    {
        t[0] = fork();
        if (t[0] == 0) 
        {
            printf("开始创建连接\n");
            for (int i = 0; ; i++)
            {
                int sock = ::socket(AF_INET, SOCK_STREAM, 0);
                assert(sock != -1);

                int ret = ::connect(sock, 
                                    serAddr.sockAddr(), 
                                    serAddr.length());
                assert(ret != -1);
                printf("%d connect succeed\n", sock);
            }
        }
    }

    while (1) {}

}
