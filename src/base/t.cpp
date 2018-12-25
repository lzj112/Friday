#include <cstring>

#include <iostream>
#include <thread>
#include <map>
#include <vector>
#include <utility>
#include <functional>

using namespace std;

void func(int id) 
{
    while (1)
    cout << id << endl;
}

int main() 
{   
    thread t1(func, 1);
    thread t2(func, 2);
    thread t3(func, 3);
    thread t4(func, 4);

    while (1)
    {}
}