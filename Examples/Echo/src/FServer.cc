#include <stdio.h>
#include <signal.h>

#include <map>

#include "ErrLog.h"
#include "FServer.h"

FServer::FServer(EpollEventLoop* baseLoop,
               const char* name,
               const char* ip,
               int port)
    : loop_(baseLoop),
      serName(name),
      serverFd(new SocketTCP()),
      serAddr(ip, port),
      threadPool(workThreadNums)
{
    serverFd->setReuseAddr();
    serverFd->setNoDely();
    serverFd->setNonBlocking();
    serverFd->bind(serAddr);
    serverFd->listen(10);
    signal(SIGPIPE, SIG_IGN);
}   

FServer::~FServer() 
{}

void FServer::starts() 
{
    MyEvent listenEvent(loop_, serverFd->fd());
    listenEvent.setReadCallBack(std::bind(&FServer::newConntion, this));
    //启动线程池,执行子线程们的loop
    //server里的loop在主线程中执行,由用户显示调用
    threadPool.start();
    
    //监听套接字 注册epoll可读事件
    loop_->regReadable(listenEvent);
}

//防止淤积,循环accept
void FServer::newConntion() 
{
    std::map<int, InitSockAddr> newConn;
    int connfd = -1;
    sockaddr_in peer;
    socklen_t peerLen = sizeof(sockaddr_in);
    //循环接收防止accept队列淤积
    while (1) 
    {
        memset(&peer, 0, peerLen);

        connfd = serverFd->accept(&peer);
        if (connfd < 0) 
            break;
        else 
        {
            InitSockAddr peerAddr(peer);
            newConn.insert(std::make_pair(connfd, peerAddr));

            printf("got a new connection fd = %d\n", connfd);
        }
    }

    for(auto x : newConn) 
    {   
        EpollEventLoop* ioLoop = threadPool.getNextLoop();
        {
            MyEvent tmpEvent(ioLoop, x.first);
            tmpEvent.setMesMgr(messMgr_);
            //将新连接fd分配给子线程
            ioLoop->regReadable(tmpEvent);
        }
    }
}

