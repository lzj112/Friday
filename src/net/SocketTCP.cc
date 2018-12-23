
#include <assert.h>

#include "../net/SocketTCP.h"

SocketTCP::SocketTCP()
{
    socketFd.setFd(creSocketTCP());
    socketFd.setNonBlocking();
}

SocketTCP::~SocketTCP() {}

int SocketTCP::creSocketTCP() 
{
    int newSocket = socket(AF_INET, SOCK_STREAM, 0);
    assert(newSocket != -1);

    return newSocket;
}

int SocketTCP::Bind(InitSockAddr localAddr) 
{
    int res = bind(socketFd.fd(),
                   static_cast<sockaddr *> (localAddr.sockAddr()),
                   localAddr.length());
    assert(res != -1);
}

int SocketTCP::Listen(int backlog) 
{
    int res = listen(socketFd.fd(), backlog);
    assert(res != -1);
}

// int SocketTCP::Accept(std::vector<int>& newConn) 
void SocketTCP::Accept(std::map<int, InitSockAddr>& newConn)
{
    newConn.clear();
    int connfd = -1;
    sockaddr_in peerAddr;
    socklen_t peerAddrLen = sizeof(peerAddr);
    while (1) 
    {
        memset(&peerAddr, 0, sizeof(sockaddr_in));

        connfd = accept(socketFd.fd(), 
                        (sockaddr *)&peerAddr, 
                        &peerAddrLen);
        if (connfd < 0) 
        {
            break;
        }
        else 
        {
            InitSockAddr tmp(peerAddr);
            newConn.insert(std::make_pair(connfd, tmp));
        }
    }
}