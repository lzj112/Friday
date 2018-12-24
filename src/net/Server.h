#ifndef SERVER_H_
#define SERVER_H_

#include <string>
#include <thread>

#include "../net/SocketTCP.h"
#include "../base/TaskQueue.h"
#include "../base/ThreadPool.h"
#include "../net/InitSockAddr.h"
#include "../net/EpollEventLoop.h"

class Server 
{
public:
    Server(EpollEventLoop* baseLoop, 
           const char* name,
           const char* ip,
           int port);
    ~Server();

    void starts();
    void newConnection(int connfd);
    void updateTask(TaskQueue<int>& queue);
    

private:
    EpollEventLoop* loop_;
    const std::string serName;
    std::unique_ptr<SocketTCP> serverFd;
    std::shared_ptr<InitSockAddr> serAddr;
    std::unique_ptr<ThreadPool> threadPool;
    TaskQueue<int> newConnFd;

};

#endif

/*
epolleventloop进行loop新连接用accept接收
放进newconnfd,副线程拿到添加进自己的epoll事件合集

在副线程中有
*/