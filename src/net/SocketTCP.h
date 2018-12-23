#ifndef SOCKETTCP_H_
#define SOCKETTCP_H_

#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <map>
#include <vector>
#include <utility>

#include "../base/FileDes.h"
// #include "../net/EpollBase.h"
#include "../net/InitSockAddr.h"

class SocketTCP 
{
public:
    SocketTCP();
    ~SocketTCP();

    int creSocketTCP();
    int fd() { return socketFd.fd(); }
    int Bind(InitSockAddr localAddr);
    int Listen(int backlog = 10);
    // int Accept(std::vector<int>&);
    void Accept(std::map<int, InitSockAddr>&);

    void setNoDely();
    void setReuseAddr();

private:
    FileDes socketFd;
};

#endif