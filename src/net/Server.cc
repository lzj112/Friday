
#include <map>

#include "../net/Server.h"

Server::Server(EpollEventLoop* baseLoop,
               const char* name,
               const char* ip,
               int port)
    : loop_(baseLoop),
      serName(name),
      serverFd(new SocketTCP()),
      serAddr(new InitSockAddr(ip, port))
    //   threadPool()
{
    serverFd->Bind(*serAddr);
    serverFd->Listen(5);
    serverFd->setReuseAddr();
    serverFd->setNonBlocking();
}   

void Server::starts() 
{
    MyEvent listenFd(serverFd->fd(), -1);
    listenFd.setReadCallBack(std::bind(&Server::newConntion, 
                                       this, 
                                       std::placeholders::_1));
    //监听套接字 注册epoll可读事件
    loop_->regReadable(listenFd);

    //启动线程池,执行子线程们的loop
    //server里的loop在主线程中执行,由用户显示调用
    threadPool->start();
}

//防止淤积,循环accept
int Server::newConntion(int) 
{
    std::map<int, InitSockAddr> newConn;
    int connfd = -1;
    sockaddr_in peer;
    socklen_t peerLen = sizeof(sockaddr_in);
    while (1) 
    {
        memset(&peer, 0, peerLen);

        connfd = serverFd->Accept();
        if (connfd < 0) 
        {
            switch(connfd)
            {
                case (EWOULDBLOCK) :  
                case (ECONNABORTED) :   
                case (EPROTO) :
                case (EINTR) :
                    break;
                default:
                {
                    //记录入日志
                    break;
                }
            }
        }
        else 
        {
            InitSockAddr peerAddr(peer);
            newConn.insert(std::make_pair(connfd, peerAddr));

            std::cout << "got a new connection fd = " << connfd;
            peerAddr.print();
        }
    }

    for(auto x : newConn) 
    {   
        EpollEventLoop* ioLoop = threadPool->getNextLoop();
        //将新连接fd分配给子线程
        ioLoop->regReadable(x.first);
    }
}

void EpollEventLoop::stopLoop() 
{
    
}
