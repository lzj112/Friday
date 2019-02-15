
#ifndef FCLIENT_H_
#define FCLIENT_H_


#include "../base/FileDes.h"
#include "../base/IOBuffer.h"
#include "../net/Connector.h"
#include "../base/TimerWheel.h"
#include "../net/EpollEventLoop.h"

class FClient 
{
public:
    FClient(EpollEventLoop* loop, 
            MyEvent* event,
            const char* ip, 
            int port);
    ~FClient();

    void bind(const char* ip, int port);    //指定绑定本地ip port
    void connect();
    void connSucceed(int sock);
    void send(int type, const char* mess);
    void send(PackageTCP tmpPack);
    // void recv

private:
    EpollEventLoop* loop_;
    std::unique_ptr<Connector> connector_;
    MyEvent* event_;
};

#endif