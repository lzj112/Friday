#include <cstring>

#include <vector>
#include <iostream>
using namespace std;

int main() 
{
    vector<char> t{'1', '2', '3', '4', '5', '\0', 'a'};
    for (auto x : t) 
        cout << x << endl;
    char str[10];
    strcpy(str, t.data());
    cout << str << endl;
}