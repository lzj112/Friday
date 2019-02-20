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
    inet_pton(AF_INET, ip, &address.sin_addr);
    address.sin_port = htons(port);
    address.sin_family = AF_INET;

    int sockfd = socket(PF_INET, SOCK_STREAM, 0);
    assert(sockfd >= 0);

    setnonblocking(sockfd);

    int ret = connect(sockfd, (struct sockaddr*)&address, sizeof(address));
    if (ret == 0)
        printf("连接成功\n");
    else 
        printf("error\n");

    ret = connect(sockfd, (struct sockaddr*)&address, sizeof(address));
    if (ret == 0)
        printf("连接成功2222\n");
    else 
        printf("%d %d\n", EISCONN, errno);


    return 0;
}