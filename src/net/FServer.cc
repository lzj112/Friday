#include <stdio.h>
#include <signal.h>

#include <map>

#include "ErrLog.h"
#include "FServer.h"

FServer::FServer(EventLoop* baseLoop,
               const char* name,
               const char* ip,
               int port)
    : loop_(baseLoop),
      serName(name),
      serverFd(new SocketTCP()),
      serAddr(ip, port)
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

void FServer::start() 
{
    std::shared_ptr<MyEvent> listenEv = 
        std::make_shared<MyEvent> (loop_, serverFd->fd());
    listenEv->setReadCallBack(std::bind(&FServer::newConntion, this));
    //监听套接字 注册epoll可读事件
    loop_->regReadable(listenEv);
    loop_->loop();
}

//防止淤积,循环accept
void FServer::newConntion() 
{
    std::vector<int> newConn;
    int connfd = -1;
    //循环接收防止accept队列淤积
    while (1) 
    {
        connfd = serverFd->accept(nullptr);
        if (connfd < 0) 
            break;
        else 
        {
            newConn.emplace_back(connfd);
            DEBUG("got a new connection fd = %d\n", connfd);
        }
    }

    for(auto x : newConn) 
    {   
        std::shared_ptr<MyEvent> myEv = 
            std::make_shared<MyEvent> (loop_, x);
        myEv->setMsgMgr(msgMgr_);
        //将新连接fd分配给子线程
        loop_->regReadable(myEv);
    }
}

