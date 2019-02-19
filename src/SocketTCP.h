#ifndef SOCKETTCP_H_
#define SOCKETTCP_H_

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

#include "FileDes.h"
#include "InitSockAddr.h"

class SocketTCP 
{
public:
    SocketTCP();
    ~SocketTCP();

    int creSocketTCP();
    int fd() { return socketFd->fd(); }
    int bind(InitSockAddr localAddr);
    int listen(int backlog = 10);
    int accept();
    int connect(InitSockAddr peerAddr);
    void close() { socketFd->close(); }
    void reSet(int fd) { socketFd->reSet(fd); }

    void setNonBlocking() { socketFd->setNonBlocking(); }
    void setNoDely();
    void setReuseAddr();
    void setKeepLive();
    int getSocketState();
    bool isSelfConnection();

private:
    std::shared_ptr<FileDes> socketFd;
};

#endif