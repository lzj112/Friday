#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <pthread.h>
#include <errno.h>

#define MAX_EVENT_NUMBER 1024
#define TCP_BUFFER_SIZE 512
#define UDP_BUFFER_SIZE 1024

int setnonblocking(int fd) 
{
    int old_option = fcntl(fd, F_GETFL); //读取文件状态标识
    int new_option = old_option | O_NONBLOCK; //设置为非阻塞
    fcntl(fd, F_SETFL, new_option); //设置文件打开方式为new_option指定的方式
    return old_option;
}

void addfdIN(int epollfd, int fd) 
{
    struct epoll_event event;
    event.data.fd = fd;
    event.events = EPOLLIN | EPOLLET; //ET模式需要非阻塞socket
    epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &event);
    setnonblocking(fd);
}

void addfdOUT(int epollfd, int fd) 
{
    struct epoll_event event;
    event.data.fd = fd;
    event.events = EPOLLOUT | EPOLLET; //ET模式需要非阻塞socket
    epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &event);
    setnonblocking(fd);
}

int main(int argc, char* argv[]) 
{
    
    // if (argc <= 2) 
    // {
    //     printf("errno\n");
    //     return 1;
    // }
    const char* ip = "127.0.0.1";
    int port = 5000;

    int ret = 0;
    struct sockaddr_in address;
    bzero(&address, sizeof(address));
    address.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &address.sin_addr);
    address.sin_port = port;

    //创建TCP socket，并将其绑定到端口port上
    int listenfd = socket(PF_INET, SOCK_STREAM, 0);
    assert(listenfd >= 0);

    ret = bind(listenfd, (struct sockaddr*)&address, sizeof(address));
    assert(ret != -1);

    ret = listen(listenfd, 5);
    assert(ret != -1);

    //创建UDP socket，并将其绑定到端口port上
    bzero(&address, sizeof(address));
    address.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &address.sin_addr);
    address.sin_port = port;
    int udpfd = socket(PF_INET, SOCK_DGRAM, 0);
    assert(udpfd >= 0);

    ret = bind(udpfd, (struct sockaddr*)&address, sizeof(address));
    assert(ret != -1);

    //epoll
    epoll_event events[MAX_EVENT_NUMBER];
    int epollfd = epoll_create(1);
    assert(epollfd != -1);

    //注册TCP、UDP socket上的可读事件
    addfdIN(epollfd, listenfd);
    addfdOUT(epollfd, listenfd);

    while (1) 
    {
        int number = epoll_wait(epollfd, events, MAX_EVENT_NUMBER, -1);
        if (number < 0) 
        {
            printf("epoll failure\n");
            break;
        }

        for (int i = 0; i < number; i++) 
        {
            int sockfd = events[i].data.fd;
            if (sockfd == listenfd) 
            {
                struct sockaddr_in client_address;
                socklen_t client_addrlength = sizeof(client_address);
                int connfd = accept(listenfd, (struct sockaddr*)&client_address, 
                                    &client_addrlength);
                addfdIN(epollfd, connfd);
            }
            else if (events[i].events & EPOLLIN) 
            {
                char buf[TCP_BUFFER_SIZE];
                while (1) 
                {
                    memset(buf, '\0', TCP_BUFFER_SIZE);
                    ret = recv(sockfd, buf, TCP_BUFFER_SIZE-1, 0);
                    if (ret < 0) 
                    {
                        if ((errno == EAGAIN) || (errno == EWOULDBLOCK)) 
                        {
                            //在非阻塞模式下调用了阻塞操作，在该操作没有完成就
                            //返回这个错误，这个错误不会破坏socket的同步，不用管它，下次循环接着recv就可以。
                            break;
                        }
                        close(sockfd);
                        break;
                    }
                    else if (ret == 0) 
                    {
                        close(sockfd);
                    }
                    else 
                    {
                        send(sockfd, buf, ret, 0);
                    }
                }
            }
            else 
            {
                printf("something wrong\n");
            }
        }
    }
    close(listenfd);
    
    return 0;
}


