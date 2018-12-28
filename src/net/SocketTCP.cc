
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
    int res = ::bind(socketFd->fd(),
                     static_cast<sockaddr *> (localAddr.sockAddr()),
                     localAddr.length());
    assert(res != -1);
}

int SocketTCP::listen(int backlog) 
{
    int res = ::listen(socketFd->fd(), backlog);
    assert(res != -1);
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

/*
ETIMEDOUT 无响应
ECONNREFUSED 收到RST 硬错误
EHOSTUNREACH ENETUNRECH 软错误,在某个路由器上不可达

*/
int SocketTCP::connect(InitSockAddr peerAddr) 
{
    errno = 0;
    int res = ::connect(socketFd->fd(), 
                        peerAddr.sockAddr(),
                        peerAddr.length());
    if (res == 0) 
        return res;
    else 
        return errno;
}

void SocketTCP::setNoDely() 
{
    int flag = 1;
    int res = setsockopt(socketFd->fd(),
                         IPPROTO_TCP,
                         TCP_NODELAY,
                         &flag,
                         sizeof(flag));
    if (res == -1) 
    {
        perror("setNoDely failed ");
    }
}

void SocketTCP::setReuseAddr() 
{
    int flag = 1;
    int res = setsockopt(socketFd->fd(),
                         IPPROTO_TCP,
                         SO_REUSEADDR,
                         &flag,
                         sizeof(flag));
    if (res == -1) 
    {
        perror("setReuseAddr falied ");
    }
}

void SocketTCP::setKeepLive() 
{
    int flag = 1;
    int res = setsockopt(socketFd->fd(),
                         IPPROTO_TCP,
                         SO_KEEPALIVE,
                         &flag,
                         sizeof(flag));
    if (res == -1) 
    {
        perror("setKeepLive failed ");
    }
}

