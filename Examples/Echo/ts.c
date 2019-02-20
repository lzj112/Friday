//用SIGURG检测带外数据是否到达

#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <pthread.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include <stdio.h>
#include <fcntl.h>
 
#define BUF_SIZE 1024

static int connfd;
int setnonblocking(int fd)
{
//设置文件描述符为非阻塞  
    	int old_option=fcntl(fd,F_GETFL, 0);  
    	int new_option=old_option|O_NONBLOCK;  
    	fcntl(fd,F_SETFL,new_option);  
    	return old_option;		//返回旧描述符以便以后恢复  
} 

int main(int argc, char* argv[]) 
{
    
    const char* ip = "127.0.0.1";
    int port = 4000;

    struct sockaddr_in address;
    bzero(&address, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    inet_pton(AF_INET, ip, &address.sin_addr);

    int sock = socket(PF_INET, SOCK_STREAM, 0);
    assert(sock >= 0);

    setnonblocking(sock);

    int ret = bind(sock, (struct sockaddr*)&address, sizeof(address));
    assert(ret != -1);

    ret = listen(sock, 5);
    assert(ret != -1);

    struct sockaddr_in client;
    socklen_t client_addrlength = sizeof(client_addrlength);
    while (1) 
    {
        connfd = accept(sock, (struct sockaddr*)&client_addrlength, &client_addrlength);
        if (connfd < 0) 
        {
        }
        else 
        {
            printf("got a new connfd = %d\n", connfd);
        }
    }
}