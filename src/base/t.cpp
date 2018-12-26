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
    const char* p ="12";
    p = "234";
    cout << p << endl;
}