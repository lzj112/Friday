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

<<<<<<< HEAD
    void setMesMgr(const mesMgr& cb) 
    { messMgr_ = cb; }
=======
    void setMesMang(const messManage& cb) 
    { messManage_ = cb; }
>>>>>>> 2dbba237396801c4e1d576a40693b98ce9c368c1
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
<<<<<<< HEAD
    mesMgr messMgr_;
=======
    messManage messManage_;
>>>>>>> 2dbba237396801c4e1d576a40693b98ce9c368c1
};

#endif
