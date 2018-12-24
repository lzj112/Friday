#include <cstring>

#include <iostream>
#include <thread>
#include <map>
#include <vector>
#include <utility>
#include <functional>

using namespace std;


struct t 
{
    int val;
    const char* ptr = "123456";
};

map<int, t> tt;
t* p;

void b(t tmp) 
{
    tt.insert(make_pair(10, tmp));
    auto it = tt.find(10);
    p = &(*it).second;
}


void a() 
{
    t tmp;
    tmp.val = 1;
    p = &tmp;
    b(tmp);
}


int main() 
{
    a();
    cout << p->val << ' ' << p->ptr << endl;

    for (auto& x : tt) 
    {
        cout << x.first << endl;
        cout << x.second.val << endl;
        cout << x.second.ptr << endl;
    }
}