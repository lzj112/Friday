
#include "../net/MyEvent.h"

MyEvent::MyEvent(EpollEventLoop* loop) 
    : fd_(-1), 
	  loop_(loop),
      ptr(nullptr),
      readCallBack_(std::bind(&MyEvent::defRead, this)),
      writeCallBack_(std::bind(&MyEvent::defWrite, this)),
      errorCallBack_(std::bind(&MyEvent::defClose, this))
{}

MyEvent::MyEvent(EpollEventLoop* loop, int fd) 
    : fd_(fd),
	  loop_(loop),
      ptr(nullptr),
      readCallBack_(std::bind(&MyEvent::defRead, this)),
      writeCallBack_(std::bind(&MyEvent::defWrite, this)),
      errorCallBack_(std::bind(&MyEvent::defClose, this))
{}

void MyEvent::goRead() //每次读取套接字上的数据时尽可能多的读取
{
	PackageTCP tmpBuffer;
	bool isEndRead = true;
	do 
	{
		bzero(&tmpBuffer, sizeof(PackageTCP));
		isEndRead = readPackHead(tmpBuffer);
		if (isEndRead) 
		{
			appendRecvBuffer(tmpBuffer);
		}
	}	while (isEndRead == true);
	readCallBack_();
}

bool MyEvent::readPackHead(PackageTCP& tmpPackage) 
{
	bzero(&tmpPackage, sizeof(PackageTCP));
	int ret = 0, sum = 0;
	bool isRecvHeadOK = true;
	while (sum < sizeof(PackHead)) //读取数据头
	{
		ret = ::recv(fd_, 
					 (&tmpPackage + sum), 
					 PACKHEADSIZE, 
					 0);
		if (ret <= 0) 
		{ isRecvHeadOK = false; break; }
		else 
			sum += ret;
	}

	bool isRecvBodyOK = true;
	if (isRecvHeadOK)
		isRecvBodyOK = readPackBody(tmpPackage, tmpPackage.head.length);	
	else 
		return false;
	if (isRecvBodyOK)
		return true;
	else 
		return false;
}

bool MyEvent::readPackBody(PackageTCP& tmp, int len) 
{
	int count = len, ret = 0, sum = 0;
	bool isRecvBodyOK = true;
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
				isRecvBodyOK = false;
				break;
			}
		}
		else
		{
			count -= ret;
			sum += ret;
		}
	}
	if (isRecvBodyOK)	//需要同时验证count<=0么?
		return true;
	else 
		return false;
}

void MyEvent::appendRecvBuffer(PackageTCP& tmp)
{
	Message tmpMess(tmp.body);
	tmpMess.setType(tmp.head.type);
	recvBuffer.appendMess(std::move(tmpMess));
}

int MyEvent::defRead() 
{
	if (!recvBuffer.isEmpty()) 
	{
		Message tmpMess;
		do 
		{
			memset(&tmpMess, 0, sizeof(Message));
			recvBuffer.readMess(tmpMess);
			std::cout << "取出数据:" << std::endl;
			tmpMess.show();
		}	while (!recvBuffer.isEmpty());
	}
}

int MyEvent::sendMess(PackageTCP& tmpPack) 
{
	Message tmpMess(tmpPack.body);
	tmpMess.setType(tmpPack.head.type);
	sendBuffer.appendMess(tmpMess);	//加入写buffer
	loop_->regWriteable(*this);	//注册写事件
}

void MyEvent::sendMess(Message tmpMess) 
{
	
}

void MyEvent::goWrite() 
{
	if (!sendBuffer.isEmpty()) 
	{
		Message tmpMess;
		do 
		{
			memset(&tmpMess, 0, sizeof(Message));
			sendBuffer.readMess(tmpMess);
			sendMess(tmpMess);
		}	while (!sendBuffer.isEmpty());
	}
}