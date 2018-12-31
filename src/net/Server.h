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
    int newConntion(int);
    // void setNewConnCallBack();
    void updateTask(TaskQueue<int>& queue);
    

private:
    EpollEventLoop* loop_;
    const std::string serName;

    std::shared_ptr<SocketTCP> serverFd;
    std::shared_ptr<InitSockAddr> serAddr;
    std::unique_ptr<ThreadPool> threadPool;
    // TaskQueue<int> newConnFd;
    // IOcallBack newConnCallBack_;

    
};

#endif
