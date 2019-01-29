
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

// #include "../net/EpollEventLoop.h"
#include "../net/Connector.h"

using namespace std;


// class A 
// {
// public:
// 	A(int v, const vector<char> tmp)
// 		: val(v), m_(tmp)
// 	{}
// 	A(const A& t) 
// 	{
// 		val = t.val;
// 		int len = t.m_.size();
// 		for (int i = 0; i < len; i++)
// 			m_.push_back(*(t.m_.data() + i));
// 	}
// 	vector<char>& ve() { return m_; }
// 	int v() const { return val; }
// 	char* beg() { return m_.data(); }
// 	void show() 
// 	{
// 		cout << val << endl;;
// 		for (auto x : m_)
// 		{
// 			cout << x << endl;
// 		}
// 	}
// private:
// 	int val;
// 	vector<char> m_;
// };

// int main() 
// {
// 	// vector<char> tmp{'a', 'b', 'c', 'd'};
// 	// A a(99, tmp);
// 	// a.show();

// 	// cout <<"************\n";
	
// 	// A b(a);
// 	// b.show();

// 	vector<int> tmp;
// 	for (int i = 0; i < 10; i++)
// 		tmp.push_back(i);
// 	cout << "size=" << tmp.size() << endl;
// 	cout << "c=" << tmp.capacity() << endl;
// }

class Test
{
public:
	Test(int v) : val(v)
	{}
	Test(const Test& t)
	{ val = 100; cout << t.val << endl; }
	void show(Test a) 
	{ cout << a.val << endl; cout << val << endl; }
	
private:
	int val;
};

int main() 
{
	Test v1(99);

	Test v2(v1);
	v1.show(v2);
}