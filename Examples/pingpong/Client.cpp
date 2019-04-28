
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

void setNonBlocking(int sockfd) 
{
    int old_option = fcntl(sockfd, F_GETFL, 0);
    int new_option = old_option | O_NONBLOCK;
    fcntl(sockfd, F_SETFL, new_option);
}

void timerFunc(int) 
{
    echo = false;
}

int main() 
{
    signal(SIGALRM, timerFunc);
    InitSockAddr serAddr("127.0.0.1", 4000);
   
    int sock = ::socket(AF_INET, SOCK_STREAM, 0);
    assert(sock != -1);

    int ret = ::connect(sock, 
                        serAddr.sockAddr(), 
                        serAddr.length());
    assert(ret != -1);
    vector<unsigned char> sendbuf(4095, 'a');
    sendbuf.push_back('\0');
    int len = 4096;

    vector<unsigned char> recvbuf(4096);

    int sendBytes = 0;
    int recvBytes = 0;
    ret = 0;
    alarm(600);
    while (echo) 
    {
        ret = ::send(sock, sendbuf.data(), len, 0);
        sendBytes += ret;
        ret =::recv(sock, recvbuf.data(), len, 0);
        recvBytes += ret;
    }

    cout << "sendBytes = " << sendBytes << endl;
    cout << "recvBytes = " << recvBytes << endl;
}
