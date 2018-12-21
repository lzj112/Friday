#ifndef SERVER_H_
#define SERVER_H_

#include <string>

#include "../net/AcceptTCP.h"
#include "../base/TaskQueue.h"
#include "../net/InitSockAddr.h"
#include "../net/EpollEventLoop.h"

class Server 
{
public:
    Server(EpollEvent* baseLoop, 
           const char* name,
           const char* ip,
           int port);
    ~Server();

    void starts();
    void newConnection(int connfd);
    

private:
    EpollEvent* loop_;
    const std::string serName;
    std::unique_ptr<AcceptTCP> accept_;
    std::unique_ptr<TaskQueue<int> > newConnFd;
    InitSockAddr serAddr;

};

#endif

/*
epolleventloop进行loop新连接用accept接收
放进newconnfd,副线程拿到添加进自己的epoll事件合集
*/