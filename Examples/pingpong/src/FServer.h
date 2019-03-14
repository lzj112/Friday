#ifndef FSERVER_H_
#define FSERVER_H_

#include <string>
#include <thread>

#include "SocketTCP.h"
#include "IOThreadPool.h"
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

    void setMsgMgr(const msgMgr& cb) 
    { msgMgr_ = cb; }
    void starts();
    int defaultConnIOCB();
    void defaultConnErrCB(); 
    
private:
    void newConntion();

    EpollEventLoop* loop_;
    const std::string serName;

    std::shared_ptr<SocketTCP> serverFd;
    InitSockAddr serAddr;
    ThreadPool threadPool;

    static const int workThreadNums = 3;

    IOcallBack connIOCallBack;
    errorCallBack connErrCallBack;
    msgMgr msgMgr_;
};

#endif
