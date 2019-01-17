
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
#include <utility>
#include <sys/timerfd.h>

using namespace std;

class A
{
public:
    A(int show) : val(show) {}
    A(const A& t) : val(t.v()) { cout << "here is &" << endl; }
    A(const A&& t) : val(t.v()) {cout << "here is &&" << endl;}

    void change(int v) { val = v; }
    void show() const { cout << "here is show"  << val << endl; }
    int v() const { return val; }
    bool operator< (const A& tmp) const
    {
        return val < tmp.v();
    }
private:
    int val;
};


int main() 
{
    // set<A> t;
    map<int, A> t;
    A a(10);
    A b(1);
    A c(5);
    A d(3);
    t.insert(make_pair(1, move(a)));
    t.insert(make_pair(3, move(b)));
    t.insert(make_pair(2, move(c)));
    t.insert(make_pair(4, move(d)));

    for (auto x = t.begin(), y = t.end(); x != y; x++) 
    {
        x->second.show();
    }

    t.begin()->second.change(0);
    for (auto x = t.begin(), y = t.end(); x != y; x++) 
    {
        x->second.show();
    }

}
