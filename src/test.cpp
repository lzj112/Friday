#include <iostream>
using namespace std;

class test
{
public:
    test(int v) 
        : val(v) 
    {}

private:
    int val;
};

void add(test&& x) 
{
    cout << "hello world" << endl;
}

void func(const test& x) 
{
    add(x);
}

s
int main() 
{
    test t(10);

}