#ifndef CONNECTOR_H_
#define CONNECTOR_H_

// #include <memory>

#include "../base/FileDes.h"
#include "../net/SocketTCP.h"
#include "../base/TimerWheel.h"
#include "../net/EpollEventLoop.h"

class Connector 
{
public:
    Connector(EpollEventLoop* baseLoop, 
              SocketTCP* sock,
              InitSockAddr* addr);
    ~Connector();

    void connect();
    void connSuccessful();
    void inConnection();
    int isConnOk();
    void reConnect();
    void gotError();

    void setReadCb(IOcallBack& cb) { readCb = cb; }
    void setWriteCb(IOcallBack& cb) { writeCb = cb; }
    void setCloseCb(errorCallBack& cb) { closeCb = cb; }

typedef std::function<void(int)> connSucced;

private:
    EpollEventLoop* loop_;
    SocketTCP* cliSock;
    InitSockAddr* serAddr;
    std::unique_ptr<TimerWheel> timerContainer;
    int reTryDelay;

    IOcallBack readCb;
    IOcallBack writeCb;
    errorCallBack closeCb;
    connSucced connSucced_;
};

#endif  
