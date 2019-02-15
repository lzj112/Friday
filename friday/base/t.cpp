
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

class Test 
{
public:
    Test(int v) : val(v) 
    {}
    void func() 
    {
        func1(this);
    }
    int v()
    { return val; }
    void func1(Test* t) 
    {
        int tmp = 1000;
        t->show(tmp, t);
    }
    void show(int tmp, Test* t) 
    {
        cout << tmp << endl;
        cout << t->v() << endl;
    }
private:
    int val;
};

int main() 
{
    Test test(99);
    test.func();
}