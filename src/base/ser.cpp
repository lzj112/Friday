

#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include <unistd.h>
#include <assert.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/timerfd.h>

#include <map>
#include <set>
#include <queue>
#include <thread>
#include <memory>
#include <vector>
#include <utility>
#include <iostream>
#include <functional>

#include "../net/Server.h"
// #include "../net/SocketTCP.h"
// #include "../net/InitSockAddr.h"
// #include "../net/EpollEventLoop.h"

using namespace std;

int main() 
{
    // EpollEventLoop loop;
    // Server ser(&loop, "serverTest", "127.0.0.1", 6000);
    // ser.starts();
    InitSockAddr addr("127.0.0.1", 6000);
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    assert(sock != -1);

    int ret = bind(sock, addr.sockAddr(), addr.length());
    assert(ret != -1);

    ret = listen(sock, 5);
    assert(ret != -1);

    sockaddr cli;
    socklen_t len = sizeof(cli);
    int connfd = accept(sock, nullptr, nullptr);
    assert(connfd != -1);

    cout << "connfd = " << connfd << endl;
}