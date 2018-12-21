
#include <memory.h>
#include <arpa/inet.h>

#include "../net/InitSockAddr.h"

InitSockAddr::InitSockAddr(const char* ip, int port) 
{
    addrLength = sizeof(netAdderss);
    memset(&netAdderss, 0, addrLength);
    
    netAdderss.sin_family = AF_INET;
    netAdderss.sin_port = htons(port);
    inet_pton(AF_INET, ip, &netAdderss.sin_addr);
}