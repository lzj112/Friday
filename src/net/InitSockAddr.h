#ifndef INITSOCKADDR_H_
#define INITSOCKADDR_H_

#include <netinet/in.h>

#include <iostream>
#include <memory.h>
#include <arpa/inet.h>


class InitSockAddr 
{
public:
    InitSockAddr(const char* ip, int port)
        : addrLength (sizeof(sockaddr_in))
    {
        memset(&netAdderss, 0, addrLength);

        netAdderss.sin_family = AF_INET;
        netAdderss.sin_port = htons(port);
        inet_pton(AF_INET, ip, &netAdderss.sin_addr);
    }

    InitSockAddr(const sockaddr_in& tmp) 
    {
        netAdderss = tmp;
        addrLength = sizeof(sockaddr_in);
    }
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