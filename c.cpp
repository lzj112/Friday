#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include <unistd.h>
#include<arpa/inet.h>
#include<sys/select.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
void select_test(int conn)
{
    int ret = 0;
    fd_set rset;
    FD_ZERO(&rset);
    int nready;
    int maxfd = conn;
    int fd_stdin = fileno(stdin);
    if(fd_stdin > maxfd)
    {
        maxfd = fd_stdin;
    }
    int len = 0;
    char readbuf[1024] = {0};
    char writebuf[1024] = {0};
    while(1)
    {
        FD_ZERO(&rset);
        FD_SET(fd_stdin, &rset);
        FD_SET(conn, &rset);
        nready = select(maxfd+1, &rset, NULL, NULL, NULL);
        if(nready == -1)
        {
            perror("select");
            exit(0);
        }
        else if(nready == 0)
        {
            continue;    
        }
        if(FD_ISSET(conn, &rset))
        {
            ret = read(conn, readbuf, sizeof(readbuf));
            if(ret == 0)
            {
                printf("server close1\n");
                break;
            }
            else if(-1 == ret)
            {
                perror("read1");
                break;
            }    
            fputs(readbuf, stdout);
            memset(readbuf, 0, sizeof(readbuf));
        }    
        
        if(FD_ISSET(fd_stdin, &rset))
        {    
            read(fd_stdin, writebuf, sizeof(writebuf));        
            len = strlen(writebuf);
            ret = write(conn, writebuf, len);
            if(ret == 0)
            {
                printf("server close3\n");
                break;
            }
            else if(-1 == ret)
            {
                perror("write");
                break;
            }
            memset(writebuf, 0, sizeof(writebuf));    
        }
    }
}
int sockfd = 0;
int main(int argc, char **argv)
{
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0)
    {
        perror("socket");
        return -1;
    }
        
    unsigned short sport = 8080;
    if(argc == 2)
    {
        sport = atoi(argv[1]);
    }
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    printf("port = %d\n", sport);
    addr.sin_port = htons(sport);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    if(connect(sockfd, (struct sockaddr*)&addr, sizeof(addr)) < 0)
    {
        perror("connect");
        return -2;
    }
    struct sockaddr_in addr2;
    socklen_t len = sizeof(addr2);
    if(getpeername(sockfd, (struct sockaddr*)&addr2, &len) < 0)
    {
        perror("getsockname");
        return -3;
    }
    printf("Server: port:%d, ip:%s\n", ntohs(addr2.sin_port), inet_ntoa(addr2.sin_addr));
    select_test(sockfd);
    close(sockfd);

    return 0;
}