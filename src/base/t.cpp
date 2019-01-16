
#include <sys/timerfd.h>
#include <cstring>

#include <iostream>
#include <thread>
#include <map>
#include <set>
#include <memory>
#include <vector>
#include <utility>
#include <functional>
#include <queue>

using namespace std;

class A 
{
public:
    A(int a, int b) : v1(a), v2(b) { cout << "here A()\n";}
    A(A&& a) : v1(a.val1()), v2(a.val2()) {}
    void show() 
    {
        cout << v1 << ' ' << v2 << endl;
    }
    int val1() const { return v1; }
    int val2() const { return v2; }
    bool operator<(const A& tmp) const 
    {
        return v1 > tmp.val1();
    } 
    A* pointer() 
    {
        return this;
    }
private:
    int v1, v2;
};


int main() 
{
    queue<A> tmp;

    A aa(1, 2);
    tmp.emplace(move(aa));
    aa.show();
    A a = tmp.front();
    a.show();
}