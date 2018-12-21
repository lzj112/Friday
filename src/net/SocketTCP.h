#ifndef SOCKETTCP_H_
#define SOCKETTCP_H_

#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "../base/FileDes.h"

class SocketTCP 
{
public:
    SocketTCP();
    ~SocketTCP();

    int fd() { return socketFd.fd(); }
    int Bind();
    int Listen();
    int Accept();


private:
    FileDes socketFd;
};

#endif