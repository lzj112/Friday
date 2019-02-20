
#include <cstring>
#include <assert.h>

#include "SocketTCP.h"

SocketTCP::SocketTCP() : socketFd(new FileDes(creSocketTCP()))
{
    //设置非阻塞
    socketFd->setNonBlocking();
}

SocketTCP::~SocketTCP()
{}

int SocketTCP::creSocketTCP() 
{
    int newSocket = ::socket(AF_INET, SOCK_STREAM, 0);
    assert(newSocket != -1);
    std::cout << "created a new socket = " << newSocket <<std::endl;
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

int SocketTCP::accept(sockaddr_in* peer)
{
    socklen_t peerAddrLen = sizeof(sockaddr_in);

    errno = 0;
    return ::accept(socketFd->fd(), 
                      (sockaddr *)peer, 
                      &peerAddrLen);
}


int SocketTCP::connect(InitSockAddr peerAddr) 
{
    errno = 0;
    return ::connect(socketFd->fd(), 
                     peerAddr.sockAddr(),
                     peerAddr.length());
}

void SocketTCP::setNoDely() 
{
    int optval = 1;
    int ret = ::setsockopt(socketFd->fd(),
                           IPPROTO_TCP,
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