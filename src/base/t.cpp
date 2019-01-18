
#include <sys/timerfd.h>

#include <iostream>
#include <thread>
#include <map>
#include <set>
#include <memory>
#include <vector>
#include <utility>
#include <functional>
#include <signal.h>
#include <queue>

#include "../base/TimerWheel.h"

using namespace std;

TimerWheel wheel;
int si = wheel.tickTime();

void func(int fd) 
{
    cout << "here is " << fd << endl;
}

void handle(int) 
{
    wheel.tick();
    alarm(si);
}

int main() 
{
    function<void(int)> f = bind(func, placeholders::_1);
    
    signal(SIGALRM, handle);
    alarm(si);

    wheel.addTimer(2, 1, f, 8086);
    wheel.addTimer(1, 0, f, 1006);
    wheel.addTimer(3, 0, f, 2000);
    wheel.addTimer(4, 1, f, 9999);

    while (1) {}
}
