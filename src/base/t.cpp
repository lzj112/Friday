
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

class test 
{
public:
	test(int v) : val(v) {}
	test* getThis() { return this; }
	void show() 
	{cout << val << endl; }
private:
	int val;
};

int main() 
{
	test t(109);
	test* ptr = t.getThis();
	ptr->show();
}