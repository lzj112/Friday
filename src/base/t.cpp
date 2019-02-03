
#include <sys/timerfd.h>

#include <iostream>
#include <thread>
#include <map>
#include <set>
#include <memory>
#include <assert.h>
#include <vector>
#include <utility>
#include <functional>
#include <signal.h>
#include <queue>

// #include "../base/Message.h"
#include "../base/PackageTCP.h"
// #include "../net/Connector.h"
// #include "../net/EpollEventLoop.h"

using namespace std;

struct Head 
{
	int type;
	int length;
};
struct Pack 
{
	Head head;
	int value;
};

void func(Pack* tmp) 
{
	cout << *tmp << endl;
}
int main() 
{
	PackageTCP t;
	cout << sizeof(t) << endl;
}