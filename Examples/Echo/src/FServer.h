#ifndef FSERVER_H_
#define FSERVER_H_

#include <string>
#include <thread>

#include "SocketTCP.h"
#include "ThreadPool.h"
#include "InitSockAddr.h"
#include "EpollEventLoop.h"
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

    void setMesMang(const messManage& cb) 
    { messManage_ = cb; }
    void starts();
    int defaultConnIOCB();
    void defaultConnErrCB(); 
    
private:
    void newConntion();

    EpollEventLoop* loop_;
    const std::string serName;

    std::shared_ptr<SocketTCP> serverFd;
    std::shared_ptr<InitSockAddr> serAddr;
    std::unique_ptr<ThreadPool> threadPool;

    static const int workThreadNums = 4;

    IOcallBack connIOCallBack;
    errorCallBack connErrCallBack;
    messManage messManage_;
};

#endif
