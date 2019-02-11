#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include <unistd.h>
#include<arpa/inet.h>
#include<sys/select.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>

#include <iostream>
using namespace std;


int sockfd = 0;
int main(int argc, char **argv)
{
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0)
    {
        perror("socket");
        return -1;
    }
        
    unsigned short sport = 8080;
    
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(sport);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    if(connect(sockfd, (struct sockaddr*)&addr, sizeof(addr)) < 0)
    {
        perror("connect");
        return -2;
    }

    char buffer[256];
    while (1) 
    {
        memset(buffer, 0, 256);
        cin >> buffer;
        send(sockfd, buffer, 256, 0);
    }
    return 0;
}