
#include <stdio.h>

#include <thread>
#include <vector>
#include <iostream>

#include "src/Connector.h"
#include "src/EpollEventLoop.h"
using namespace std;

sockaddr_in netAdderss;
socklen_t addrLength = sizeof(sockaddr_in);


int main() 
{
    const char* ip = "127.0.0.1";
    int port = 4000;
    memset(&netAdderss, 0, addrLength);

    netAdderss.sin_family = AF_INET;
    netAdderss.sin_port = htons(port);
    inet_pton(AF_INET, ip, &netAdderss.sin_addr);

    // pid_t t[3];
    // for (int i = 0; i < 3; i++) 
    // {
    //     t[i] = fork();
    //     if (t[i] == 0) 
    //     {
            array<int, 1000> sockfd;
            for (int i = 0; i < 1000; i++)
            {
                sockfd[i] = socket(AF_INET, SOCK_STREAM, 0);
                // int socketfd = socket(AF_INET, SOCK_STREAM, 0);
                // assert(socketfd != -1);
                printf("create socket = %d\n", sockfd[i]);
                // sockfd.push_back(socketfd);
                int ret = connect(sockfd[i], (sockaddr *)&netAdderss, addrLength);
                if (ret == 0)
                {
                    printf("%d connect succeed\n", sockfd[i]);
                    // char ctr[] = "hello";
                    // PackageTCP pac(1, ctr);
                    // ret = send(sockfd[i], &pac, sizeof(PackageTCP), 0);
                    // if (ret <= 0)
                    //     printf("send failed\n");
                }
                else 
                    printf("%dconnect failed\n", sockfd[i]);
    //         }
    //     }
    //     else 
    //     ;
            }

    // while (1) {}

}
