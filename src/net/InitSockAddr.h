#ifndef INITSOCKADDR_H_
#define INITSOCKADDR_H_

#include <netinet/in.h>

#include <iostream>

class InitSockAddr 
{
public:
    InitSockAddr(const char* ip, int port);

    socklen_t length() { return addrLength; }
    sockaddr* sockAddr() 
    { 
        // return static_cast<struct sockaddr*> (static_cast<void *> (&netAdderss)); 
        return reinterpret_cast<struct sockaddr*> (&netAdderss);
    }

private:
    sockaddr_in netAdderss;
    socklen_t addrLength;
};


#endif