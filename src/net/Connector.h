#ifndef CONNECTOR_H_
#define CONNECTOR_H_

// #include <memory>

#include "../base/FileDes.h"
#include "../net/SocketTCP.h"
#include "../net/EpollEventLoop.h"

class Connector 
{
public:
    Connector(EpollEventLoop* baseLoop, 
              SocketTCP* p,
              InitSockAddr* addr);
    ~Connector();

    void connect();
    void connSuccessful();
    void inConnection();
    int isConnOk(int);
    void reConnect();
    void gotError(int);

    void setReadCb(IOcallBack& cb) { readCb = cb; }
    void setWriteCb(IOcallBack& cb) { writeCb = cb; }
    void setCloseCb(closeCallBack& cb) { closeCb = cb; }

private:
    EpollEventLoop* loop_;
    SocketTCP* cliSock;
    InitSockAddr* serAddr;
    int reTryDelay;

    IOcallBack readCb;
    IOcallBack writeCb;
    closeCallBack closeCb;
    
};

#endif  