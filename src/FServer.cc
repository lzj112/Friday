
#include <map>

#include "FServer.h"

FServer::FServer(EpollEventLoop* baseLoop,
               const char* name,
               const char* ip,
               int port)
    : loop_(baseLoop),
      serName(name),
      serverFd(new SocketTCP()),
      serAddr(new InitSockAddr(ip, port)),
      threadPool( new ThreadPool(workThreadNums))
{
    serverFd->bind(*serAddr);
    serverFd->listen(5);
    serverFd->setReuseAddr();
    serverFd->setNoDely();
    serverFd->setNonBlocking();
}   

FServer::~FServer() 
{}

void FServer::starts() 
{
    MyEvent listenEvent(loop_, serverFd->fd());
    listenEvent.setMessMana(std::bind(&FServer::newConntion, 
                                       this,
                                       std::placeholders::_1,
                                       std::placeholders::_2));
    //启动线程池,执行子线程们的loop
    //server里的loop在主线程中执行,由用户显示调用
    threadPool->start();
    
    //监听套接字 注册epoll可读事件
    loop_->regReadable(listenEvent);
}

//防止淤积,循环accept
void FServer::newConntion(const MyEvent*, const Message&) 
{
    printf("有新的连接等待accept\n");
    std::map<int, InitSockAddr> newConn;
    int connfd = -1;
    sockaddr_in peer;
    socklen_t peerLen = sizeof(sockaddr_in);
    //循环接收防止accept队列淤积
    while (1) 
    {
        memset(&peer, 0, peerLen);

        connfd = serverFd->accept();
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
        {
            MyEvent tmpEvent(ioLoop, x.first);
            tmpEvent.setMessMana(messManage_);
            //将新连接fd分配给子线程
            ioLoop->regReadable(tmpEvent);
        }
    }
}

