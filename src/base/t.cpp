#include <cstring>

#include <iostream>
#include <thread>
#include <map>
#include <memory>
#include <vector>
#include <utility>
#include <functional>

using namespace std;

int a = 1;

int& func() 
{
    return a;
}

int main() 
{
    function<void(int)> t;
    if (t == nullptr) 
    {
        cout << "asasas" << endl;
    } 
}