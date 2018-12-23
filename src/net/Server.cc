
#include "../net/Server.h"

Server::Server(EpollEventLoop* baseLoop,
               const char* name,
               const char* ip,
               int port)
    : loop_(baseLoop),
      serName(name),
      serAddr(new InitSockAddr(ip, port))
{
    // serverFd
}