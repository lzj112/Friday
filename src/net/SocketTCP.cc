
#include <assert.h>

#include "../net/SocketTCP.h"

SocketTCP::SocketTCP() : socketFd(new FileDes(creSocketTCP()))
{
    // socketFd.setFd(creSocketTCP());
    socketFd->setNonBlocking();
}

SocketTCP::~SocketTCP() {}

int SocketTCP::creSocketTCP() 
{
    int newSocket = socket(AF_INET, SOCK_STREAM, 0);
    assert(newSocket != -1);

    return newSocket;
}

int SocketTCP::bind(InitSockAddr localAddr) 
{
    int ret = ::bind(socketFd->fd(),
                     static_cast<sockaddr *> (localAddr.sockAddr()),
                     localAddr.length());
    assert(ret != -1);
}

int SocketTCP::listen(int backlog) 
{
    int ret = ::listen(socketFd->fd(), backlog);
    assert(ret != -1);
}

int SocketTCP::accept()
{
    sockaddr_in peerAddr;
    memset(&peerAddr, 0, sizeof(sockaddr_in));
    socklen_t peerAddrLen = sizeof(peerAddr);

    errno = 0;
    int connfd = -1;
    connfd = ::accept(socketFd->fd(), 
                      (sockaddr *)&peerAddr, 
                      &peerAddrLen);
    
    if (connfd < 0) 
        return errno;
    else 
        return connfd;
}


int SocketTCP::connect(InitSockAddr peerAddr) 
{
    errno = 0;
    int ret = ::connect(socketFd->fd(), 
                        peerAddr.sockAddr(),
                        peerAddr.length());
    if (ret == 0) 
        return ret;
    else 
        return errno;
}

void SocketTCP::setNoDely() 
{
    int flag = 1;
    int ret = setsockopt(socketFd->fd(),
                         IPPROTO_TCP,
                         TCP_NODELAY,
                         &flag,
                         sizeof(flag));
    if (ret == -1) 
    {
        perror("setNoDely failed ");
    }
}

void SocketTCP::setReuseAddr() 
{
    int flag = 1;
    int ret = setsockopt(socketFd->fd(),
                         IPPROTO_TCP,
                         SO_REUSEADDR,
                         &flag,
                         sizeof(flag));
    if (ret == -1) 
    {
        perror("setReuseAddr falied ");
    }
}

void SocketTCP::setKeepLive() 
{
    int flag = 1;
    int ret = setsockopt(socketFd->fd(),
                         IPPROTO_TCP,
                         SO_KEEPALIVE,
                         &flag,
                         sizeof(flag));
    if (ret == -1) 
    {
        perror("setKeepLive failed ");
    }
}

