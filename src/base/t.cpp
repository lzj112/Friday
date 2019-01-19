
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

#include "../base/TimerWheel.h"

using namespace std;

// class A 
// {
// public:
//     A(int a) : val(a) {}
//     int v() const { return val; }
//     bool operator<(const A& t) const 
//     {
//         return val < t.v();
//     } 
// private:
//     int val;
// };

// int main() 
// {
//     set<A> w;
//     auto it = w.emplace(10);
//     assert(it.second == true);

//     it = w.emplace(19);
//     assert(it.second == true);

//     w.erase(10);

//     it = w.emplace(10);
//     assert(it.second == true);


// }

TimerWheel wheel;
int si = wheel.tickTime();

void func(int fd) 
{
    cout << "here is------------------ " << fd << endl;
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
    wheel.addTimer(4, 1, f, 9999);

    while (1) {}
}
