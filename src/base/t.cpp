#include <cstring>

#include <iostream>
#include <thread>
#include <vector>


#include "Message.h"

using namespace std;

class TT 
{
public:
    TT() : a(10) {}
    ~TT() { cout << "~TT" << endl; }
    void show() { cout << a << endl; }
    int a;
};
class T 
{
public:
    T(TT* pp) : p(pp) {}
    ~T() { cout << "~T" << endl; }
    void show() 
    {
        p->show();
    }
private:
    shared_ptr<TT> p;
};

int main() 
{
    TT* a = new TT();
    T tmp(a);
    tmp.show();
}