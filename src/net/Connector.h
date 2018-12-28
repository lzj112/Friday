#ifndef CONNECTOR_H_
#define CONNECTOR_H_

#include "../base/FileDes.h"
#include "../net/EpollEventLoop.h"

class Connector 
{
public:
    Connector(EpollEventLoop* baseLoop);
    ~Connector();

    void connect();
    void reConnect();
private:
    EpollEventLoop* loop_;
};

#endif