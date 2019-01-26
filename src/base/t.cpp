#include <iostream>
#include <vector>
#include <string>
using namespace std;

class Test
{
public:
	Test(const string& s = "hello world") :str(new string(s)) { cout << "默认构造函数" << endl; };
	Test(const Test& t);
	Test& operator=(const Test& t);
	Test(Test&& t)noexcept;
	Test& operator=(Test&& t)noexcept;
	~Test();
public:
	string * str;
};

Test::Test(const Test& t)
{
	str = new string(*(t.str));
	cout << "拷贝构造函数" << endl;
}
Test& Test::operator=(const Test& t)
{
	cout << "拷贝赋值运算符" << endl;
	return *this;
}
Test::Test(Test&& t)noexcept
{
	str = t.str;
	t.str = nullptr;
	cout << "移动构造函数" << endl;
}
Test& Test::operator=(Test&& t)noexcept
{
	cout << "移动赋值运算符" << endl;
	return *this;
}
Test::~Test()
{
	cout << "析构函数" << endl;
}

int main()
{
	vector<Test> vec(1);
	Test t("what");
	vec.push_back(std::move(t)); 
	return 0;
}
// #include <sys/timerfd.h>

// #include <iostream>
// #include <thread>
// #include <map>
// #include <set>
// #include <memory>
// #include <assert.h>
// #include <vector>
// #include <utility>
// #include <functional>
// #include <signal.h>
// #include <queue>

// // #include "../net/EpollEventLoop.h"
// #include "../net/Connector.h"

// using namespace std;

// class A 
// {
// public:
//     A() { cout << "构造函数" << endl; }
//     A(A&&) { cout << "移动构造" << endl; }
//     A(const A&) { cout << "拷贝构造" << endl; }
//     A getTmp() 
//     {
//         A tmp;
//         return tmp;
//     }
// };

// A func(A&& tmp) 
// {
//     A t;
//     return t;
// }

// int main() 
// {
//     // EpollEventLoop loop;
//     // SocketTCP sock;
//     // InitSockAddr addr("127.0.0.1", 6000);
//     // Connector conn(&loop, &sock, &addr);
//     // conn.connect();
//     A a;
//     a.getTmp();
// }