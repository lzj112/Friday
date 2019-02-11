/*
#include <sys/timerfd.h>

#include <iostream>
#include <thread>
#include <map>
#include <set>
#include <memory>
#include <assert.h>
#include <vector>
#include <utility>
#include <functional>
#include <signal.h>
#include <queue>

// #include "../base/Message.h"
#include "../base/PackageTCP.h"
// #include "../net/Connector.h"
// #include "../net/EpollEventLoop.h"

using namespace std;

*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>			/* socket类定义需要*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>
#include <errno.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include  <arpa/inet.h>
#include <sys/select.h>
#include <iostream>
using namespace std;

const int MAXNUM = 20;

struct Myevent 
{
    int fd;
    void* ptr;
};

int main(int argc, char *argv[])
{
//!> 建立套接字
    int connfd;
	if( ( connfd = socket( AF_INET, SOCK_STREAM, 0 ) ) == -1 )
	{
		printf("Socket Error...\n" , errno );
		exit( EXIT_FAILURE );
	}
    
    sockaddr_in servaddr;
	//!> 套接字信息
	bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(8080);
    inet_pton(AF_INET, "127.0.0.1", &servaddr.sin_addr);
	
    int epollfd = epoll_create(1);  //创建epoll实例对象

    Myevent tmp;
    tmp.fd = connfd;
    struct epoll_event ev;
    struct epoll_event events[MAXNUM];
    ev.data.ptr= &tmp;
    ev.events = EPOLLIN | EPOLLET;
    epoll_ctl(epollfd, EPOLL_CTL_ADD, timefd, &ev); //添加到epoll事件集合

    for (; ;) 
    {
        int num = epoll_wait(epollfd, events, MAXNUM, 0);
        assert(num >= 0);

        for (int i = 0; i < num; i++) 
        {
            if (events[i].events & EPOLLIN) 
            {
                //....处理其他事件
                if (events[i].data.fd == timefd) 
                {
                    s = read(events[i].data.fd, &exp, sizeof(uint64_t)); //需要读出uint64_t大小, 不然会发生错误
                    assert(s == sizeof(uint64_t));
                    cout << "here is timer" << endl;
                }
            }
        }
    }

    close(timefd);
    close(epollfd);

    return 0;
}