
#pragma once

#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <map>
#include <memory>
#include <vector>
#include <utility>
#include <linux/tcp.h>

// #include "FileDes.h"
#include "InitSockAddr.h"


class SocketTCP 
{
public:
    SocketTCP();
    ~SocketTCP();

    int creSocketTCP();
    int fd() { return sockfd; }
    int bind(InitSockAddr localAddr);
    int listen(int backlog = 10);
    int accept(sockaddr_in* peer);
    int connect(InitSockAddr peerAddr);
    void close() { ::close(sockfd); }
    void reSet();

    void setNonBlocking();
    void setNoDely();
    void setReuseAddr();
    void setKeepLive();
    int getSocketState();
    bool isSelfConnection();

private:
    // std::shared_ptr<FileDes> socketFd;
    int sockfd;
};

