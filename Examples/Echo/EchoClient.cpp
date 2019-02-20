
#include <stdio.h>

#include "src/Connector.h"
#include "src/EpollEventLoop.h"

void newConn(int connfd) 
{
    printf("新的连接已经建立,new connfd = %d\n", connfd);    
}
int main() 
{
    EpollEventLoop loop;
    SocketTCP sock;
    Connector connector_(&loop, 
                         &sock,
                         "127.0.0.1",
                         4000);
    connector_.setconnSucc(newConn);
    connector_.connect();
    loop.loop();
}

// #include <sys/socket.h>
// #include <netinet/in.h>
// #include <arpa/inet.h>
// #include <assert.h>
// #include <stdio.h>
// #include <unistd.h>
// #include <stdlib.h>
// #include <string.h>
 
// int main(int argc, char* argv[]) 
// {
   
//     const char* ip = "127.0.0.1";
//     int port = 4000;
    
//     struct sockaddr_in address;
//     bzero(&address, sizeof(address));
//     inet_pton(AF_INET, ip, &address.sin_addr);
//     address.sin_port = htons(port);
//     address.sin_family = AF_INET;

//     int sockfd = socket(PF_INET, SOCK_STREAM, 0);
//     assert(sockfd >= 0);
//     printf("new clisock = %d\n", sockfd);

//     if (connect(sockfd, (struct sockaddr*)&address, sizeof(address)) < 0) 
//     {
//         printf("connection failed\n");
//     }
//     else 
//     {
//         printf("连接成功\n");
//     }
//     close(sockfd);
//     return 0;
// }