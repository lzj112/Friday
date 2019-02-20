#ifndef CONNECTOR_H_
#define CONNECTOR_H_

// #include <memory>

#include "FileDes.h"
#include "SocketTCP.h"
#include "TimerWheel.h"
#include "EpollEventLoop.h"


class Connector 
{
public:
typedef std::function<void(int)> connSucced;

    Connector(EpollEventLoop* loop,
              SocketTCP* sock,
              const char* ip,
              int port);
    ~Connector()
    {}

    void connect();
    void connSuccessful();
    void inConnection();
    void isConnOk();
    void reConnect();
    void gotError();

    void setconnSucc(const connSucced& cb) 
    { connSucced_ = cb; }
    void setReadCb(IOcallBack& cb) 
    { readCb = cb; }
    void setWriteCb(IOcallBack& cb) 
    { writeCb = cb; }
    void setCloseCb(errorCallBack& cb) 
    { closeCb = cb; }


private:
    EpollEventLoop* loop_;
    SocketTCP* cliSock;
    std::unique_ptr<InitSockAddr> serAddr;
    std::unique_ptr<TimerWheel> timerContainer;

    int reTryDelay;
    
    static const int maxCEILING = 30000;
    static const int initReTryDelay = 500;

    IOcallBack readCb;
    IOcallBack writeCb;
    errorCallBack closeCb;
    connSucced connSucced_;
};

#endif  