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

using namespace std;

class T 
{
public:
    T() 
    {
        cout << "here is T()" << endl;
    }
    T(const T&) 
    {
        cout << "here is copy" << endl;
    }
    ~T() 
    {
        cout << "here is ~T()" << endl;
    }
private:
};

int main() 
{
    map<int, T> tmp;

    tmp.insert(make_pair(1, T()));
    // tmp.erase(1);
    tmp.insert(make_pair(2, T()));
    cout << "``````````````````````" << endl;
    // tmp.clear();
    tmp.erase(1);
    tmp.erase(2);
    // while (1) {}
}