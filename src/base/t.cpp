
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

// #include "../net/EpollEventLoop.h"
#include "../net/Connector.h"

using namespace std;

int main() 
{
    EpollEventLoop loop;
    SocketTCP sock;
    InitSockAddr addr("127.0.0.1", 6000);
    Connector conn(&loop, &sock, &addr);
    conn.connect();
}