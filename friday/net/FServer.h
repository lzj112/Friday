#ifndef FSERVER_H_
#define FSERVER_H_

#include <string>
#include <thread>

#include "../net/SocketTCP.h"
#include "../base/TaskQueue.h"
#include "../base/ThreadPool.h"
#include "../net/InitSockAddr.h"
#include "../net/EpollEventLoop.h"
/*
新连接connfd要设置为非阻塞
*/
class FServer 
{
public:
    FServer(EpollEventLoop* baseLoop, 
           const char* name,
           const char* ip,
           int port);
    ~FServer();

    void starts();
    int defaultConnIOCB();
    void defaultConnErrCB(); 
    
private:
    int newConntion();

    EpollEventLoop* loop_;
    const std::string serName;

    std::shared_ptr<SocketTCP> serverFd;
    std::shared_ptr<InitSockAddr> serAddr;
    std::unique_ptr<ThreadPool> threadPool;

    IOcallBack connIOCallBack;
    errorCallBack connErrCallBack;
};

#endif
