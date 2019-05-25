#pragma once

#include <string>
#include <thread>

#include "SocketTCP.h"
#include "FThreadPool.h"
// #include "IOThreadPool.h"
#include "InitSockAddr.h"
#include "EventLoop.h"
/*
新连接connfd要设置为非阻塞
*/
class FServer 
{
public:
    FServer(EventLoop* baseLoop, 
            const char* name,
            const char* ip,
            int port);
    ~FServer();

    void setMsgMgr(const msgMgr& cb) 
    { msgMgr_ = cb; }
    void start();
    int defaultConnIOCB();
    void defaultConnErrCB(); 
    
private:
    void newConntion();

private:
    EventLoop* loop_;
    const std::string serName;

    std::shared_ptr<SocketTCP> serverFd;
    InitSockAddr serAddr;

    IOcallBack connIOCallBack;
    errorCallBack connErrCallBack;
    msgMgr msgMgr_;
};

