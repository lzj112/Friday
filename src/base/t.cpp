#include <cstring>

#include <iostream>
#include <thread>
#include <map>
#include <memory>
#include <vector>
#include <utility>
#include <functional>

using namespace std;

class A 
{
public:
    A() : val(107), v2(new int(10))
    { }
    A(int v) : val(v), v2(new int(v))
    { }
    void show() 
    {
        cout << val << endl;
        cout << v2 << endl;
    }
    A(const A& t) 
    {

    }
private:
    int val;
    shared_ptr<int> v2;
};

void release(int val) 
{
    cout << val << endl;
}

void func(shared_ptr<int>& t) 
{
    shared_ptr<int> p1(new int(1024), bind(release, 1));
    swap(p1, t);
    cout << "here is func " << *p1 << endl;
}
int main() 
{
    // shared_ptr<int> p1(new int(1024), bind(release, 1));
    shared_ptr<int> p2(new int(2048), bind(release, 2));
    // std::swap(p1, p2);/
    // cout << *p1 << endl;
    cout << "here is main " << *p2 << endl;
    func(p2);

    cout << "after func " << *p2 << endl;
}