
#include <stdio.h>
#include <assert.h>

#include <iostream>
#include <vector>

#include "src/InitSockAddr.h"
#include "src/EpollEventLoop.h"
using namespace std;

static const int INITSOCKMAX = 1;   //1 / 10 / 100 / 1000

void pingpongFunc(MyEvent* ev, Message& m) 
{
    ev->sendMess(m);
}

void setNonBlocking(int sockfd) 
{
    int old_option = fcntl(sockfd, F_GETFL, 0);
    int new_option = old_option | O_NONBLOCK;
    fcntl(sockfd, F_SETFL, new_option);
}

int main() 
{
    InitSockAddr serAddr("127.0.0.1", 6000);
   
    EpollEventLoop loop;

    Message mess("Hello World!");
    mess.setType(123);

    vector<int> cliSock; 
    int sock;
    for (int i = 0; i < INITSOCKMAX; i++) 
    {
        sock = ::socket(AF_INET, SOCK_STREAM, 0);
        
        assert(sock != -1);
        cliSock.push_back(sock);
        int ret = ::connect(cliSock[i], 
                            serAddr.sockAddr(), 
                            serAddr.length());
        assert(ret != -1);
        // send(cliSock[i], &pack, sizeof(PackageTCP), 0);
        setNonBlocking(sock);

        {
            MyEvent event(&loop, cliSock[i]);
            event.setMessMana(pingpongFunc);
            loop.regReadable(event);
            event.sendMess(mess);
        }
    }

    loop.loop();
}