
#ifndef CLIENT_H_
#define CLIENT_H_


#include "../base/FileDes.h"
#include "../base/IOBuffer.h"
#include "../net/Connector.h"
#include "../base/TimerWheel.h"
#include "../net/EpollEventLoop.h"

class Client 
{
public:
    Client();
    Client(const char* ip, int port);   //指定本地ip port
    ~Client();

    void bind(const char* ip, int port);    //指定绑定本地ip port
    void connect();


private:

};

#endif