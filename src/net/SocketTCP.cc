
#include <cstring>
#include <assert.h>

#include "../net/SocketTCP.h"

SocketTCP::SocketTCP() : socketFd(new FileDes(creSocketTCP()))
{
    // socketFd.setFd(creSocketTCP());
    socketFd->setNonBlocking();
}

SocketTCP::~SocketTCP()
{ delete socketFd; }

int SocketTCP::creSocketTCP() 
{
    int newSocket = ::socket(AF_INET, SOCK_STREAM, 0);
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
    int optval = 1;
    int ret = ::setsockopt(socketFd->fd(),
                           SOL_SOCKET,
                           TCP_NODELAY,
                           &optval,
                           static_cast<socklen_t> (sizeof(int)));
    if (ret == -1) 
    {
        perror("setNoDely failed ");
    }
}

void SocketTCP::setReuseAddr() 
{
    int optval = 1;
    int ret = ::setsockopt(socketFd->fd(),
                           SOL_SOCKET,
                           SO_REUSEADDR,
                           &optval,
                           static_cast<socklen_t> (sizeof(optval)));
    if (ret == -1) 
    {
        perror("setReuseAddr falied ");
    }
}

void SocketTCP::setKeepLive() 
{
    int optval = 1;
    int ret = ::setsockopt(socketFd->fd(),
                           SOL_SOCKET,
                           SO_KEEPALIVE,
                           &optval,
                           static_cast<socklen_t> (sizeof(optval)));
    if (ret == -1) 
    {
        perror("setKeepLive failed ");
    }
}

int SocketTCP::getSocketState() 
{
    int optval;
    socklen_t optlen = static_cast<socklen_t> (sizeof(optval));
    int ret = ::getsockopt(socketFd->fd(),
                           SOL_SOCKET,
                           SO_ERROR,
                           &optval,
                           &optlen);
    if (ret < 0) 
        return errno;
    else 
        return optval;
}

bool SocketTCP::isSelfConnection() 
{
    struct sockaddr_in local, peer;
    socklen_t len = sizeof(sockaddr_in);
    memset(&local, 0, len);
    memset(&peer, 0, len);

    getpeername(socketFd->fd(), 
                (struct sockaddr *)&peer,
                &len);
    
    getsockname(socketFd->fd(),
                (struct sockaddr *)&local,
                &len);
    return (peer.sin_port == local.sin_port) &&
        (peer.sin_addr.s_addr == local.sin_addr.s_addr);
}