
#include "../net/MyEvent.h"

MyEvent::MyEvent() 
    : fd_(-1), 
      ptr(nullptr),
      readCallBack_(std::bind(&MyEvent::defRead, this)),
      writeCallBack_(std::bind(&MyEvent::defWrite, this)),
      errorCallBack_(std::bind(&MyEvent::defClose, this))
{}

MyEvent::MyEvent(int fd) 
    : fd_(fd),
      ptr(nullptr),
      readCallBack_(std::bind(&MyEvent::defRead, this)),
      writeCallBack_(std::bind(&MyEvent::defWrite, this)),
      errorCallBack_(std::bind(&MyEvent::defClose, this))
{}

void MyEvent::goRead() 
{
	PackageTCP tmpPackage;
	bzero(&tmpPackage, sizeof(PackageTCP));
	int ret = 0;
	int sum = 0;
	while (sum < sizeof(PackHead)) 
	{
		ret = ::recv(fd_, 
					 (&tmpPackage + sum), 
					 sizeof(PackHead), 
					 0);
		if (ret <= 0) 
			break;
		else 
			sum += ret;
	}

	readPackBody(tmpPackage, tmpPackage.head.length);	
}

void MyEvent::readPackBody(PackageTCP& tmp, int len) 
{
	int count = len;
	int ret = 0;
	int sum = 0;
	while (count > 0) 
	{
		ret = ::recv(fd_, (tmp.body + sum), len, 0);
		if (ret <= 0) 
		{
			if (errno == EINTR ||
				errno == EWOULDBLOCK) 
			{
				ret = 0;
				continue;
			}
			else 
			{
				//断开连接
				//关闭 fd 删除 event
				break;

			}
		}
		else
		{
			count -= ret;
			sum += ret;
		}
	}

	if (count <= 0) 
	{
		/*
		在这个函数里添加完处理?
		感觉不是很合理,那不和一次读取一个处理一个一样
		是每次尽量多读取PackageTCP然后存入iobuffer?
		还是处理不在添加完就进行?
		*/
		appendPack();
	}
}
