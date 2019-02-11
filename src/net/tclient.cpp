#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include  <arpa/inet.h>
#include <sys/select.h>
#include <iostream>
using namespace std;

#define MAXLINE 1024
 
int main()
{
    char * SERV_IP = "127.0.0.1";
	char 	buf[MAXLINE];
	int   	connfd;
	struct sockaddr_in servaddr;
	
	
	//!> 建立套接字
	if( ( connfd = socket( AF_INET, SOCK_STREAM, 0 ) ) == -1 )
	{
		printf("Socket Error...\n" , errno );
		exit( EXIT_FAILURE );
	}
 
	//!> 套接字信息
	bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(8080);
    inet_pton(AF_INET, SERV_IP, &servaddr.sin_addr);
	
	//!> 链接server
	if( connect( connfd, ( struct sockaddr *  )&servaddr, sizeof( servaddr ) ) < 0 )
	{
		printf("Connect error..\n");
		exit(EXIT_FAILURE);
	}	
    while (1) 
    {
        bzero(buf, sizeof(buf));
        cin >> buf;
        send(connfd, buf, sizeof(buf), 0);
    }
 
	close( connfd );
	printf("Exit\n");
	
	return 0;
}
