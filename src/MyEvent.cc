#include <stdio.h>

#include "MyEvent.h"


MyEvent::MyEvent(EpollEventLoop* loop, int fd) 
    : fd_(fd),
	  loop_(loop),
      ptr(nullptr),
	  readCallBack_(nullptr),
	  writeCallBack_(nullptr),
	  messManage_(nullptr),
	  errorCallBack_(nullptr)
{}


MyEvent::MyEvent(const MyEvent& t) 
	: fd_(t.fd_),
	  loop_(t.loop_),
	  ptr(t.ptr),
	  readCallBack_(t.readCallBack_),
	  writeCallBack_(t.writeCallBack_),
	  messManage_(t.messManage_),
	  errorCallBack_(t.errorCallBack_)
{
	sendBuffer = t.sendBuffer;
	recvBuffer = t.recvBuffer;
}

void MyEvent::goRead() //每次读取套接字上的数据时尽可能多的读取
{
	if (readCallBack_ == nullptr)
	{
		PackageTCP tmpBuffer;
		bool isEndRead = true;
		do 
		{
			bzero(&tmpBuffer, sizeof(PackageTCP));
			isEndRead = readPackHead(&tmpBuffer);
			if (isEndRead)
				isEndRead = readPackBody(tmpBuffer, tmpBuffer.head.length);
			if (isEndRead) 
			{
				printf("将[%s]添加到recvbuffer\n", tmpBuffer.body);
				appendRecvBuffer(tmpBuffer);
			}
		}	while (isEndRead == true);

		//处理拿到的数据
		performMessManaCB();
	}
	else 
		readCallBack_();
}

bool MyEvent::readPackHead(PackageTCP* pack) 
{
	int ret = 0, sum = 0;
	bool isRecvHeadOK = true;
	while (sum < PACKHEADSIZE) //读取数据头
	{
		ret = ::recv(fd_, 
					 (pack + sum), 
					 PACKHEADSIZE, 
					 0);
		if (ret <= 0) 
		{ isRecvHeadOK = false; break; }
		else 
			sum += ret;
	}

	if (isRecvHeadOK)
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
		ret = ::recv(fd_, 
					 (tmp.body + sum), 
					 MAXBODY,
					 0);
		if (ret < 0) 
		{
			if (errno == EINTR || 
			 	errno == EAGAIN)
				continue;
			else 
				break;

		}
		else if (ret == 0) 
		{
			printf("断开连接\n");
			::close(fd_);
			isRecvBodyOK = false;
			break;
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

//读取完数据后挨个处理
//messManage_为用户指定
void MyEvent::performMessManaCB() 
{
	if (!recvBuffer.isEmpty()) 
	{
		Message tmpMess;
		do 
		{
			memset(&tmpMess, 0, sizeof(Message));
			recvBuffer.readMess(tmpMess);
			if (messManage_ != nullptr)
			{
				messManage_(this, tmpMess);
			}
			else 
			{
				std::cout << "take the message:" << std::endl;
				tmpMess.show();
			}
		}	while (!recvBuffer.isEmpty());
	}
}

void MyEvent::goWrite() 
{
	if (writeCallBack_ != nullptr) 
		writeCallBack_();
	else if (!sendBuffer.isEmpty()) 
	{
		Message tmpMess;
		do 
		{
			memset(&tmpMess, 0, sizeof(Message));
			sendBuffer.readMess(tmpMess);
			printf("从sendbuffer中拿到%s\n", tmpMess.mess());
			sendMessTo(tmpMess);
		}	while (!sendBuffer.isEmpty());
	}
	//改为监听可读事件
	changeToIN();
}


int MyEvent::sendMess(Message mess)
{
	sendBuffer.appendMess(std::move(mess));	//加入写buffer

	changeToOUT();
}

//重新添加 注册可写
void MyEvent::sendMessTo(Message tmpMess) 
{
	PackageTCP package;
    strcpy(package.body, tmpMess.mess());
    package.head.type = tmpMess.type();
    package.head.length = sizeof(package.body);
	
	bool isSucSend = sendMessHead(&package);
	if (isSucSend == true)
		isSucSend = sendMessBody(&package + PACKHEADSIZE, package.head.length);
	if (isSucSend == false) 
	{
		// sendMess(tmpMess);
		std::cout << "发送一条消息失败" << std::endl;
	}
}

bool MyEvent::sendMessHead(PackageTCP* pac) 
{
	int count = PACKHEADSIZE, ret = 0, sum = 0;
	while (count > 0) 
	{
		ret = send(fd_,
				   (pac + sum),
				   count,
				   0);
		if (ret <= 0) 
		{
			if (errno == EAGAIN || 
				errno == EWOULDBLOCK || 
				errno == EINTR)
				continue;
			else 
			{
				return false;
			}
		}
		else 
		{
			count -= ret;
			sum += ret;
		}
	}
	return true;
}

bool MyEvent::sendMessBody(PackageTCP* pac, int length) 
{
	int count = length, ret = 0, sum = 0;
	while (count > 0) 
	{
		ret = send(fd_,
				   (pac + sum),
				   count,
				   0);
		if (ret <= 0) 
		{
			if (errno == EAGAIN || 
				errno == EWOULDBLOCK || 
				errno == EINTR)
				continue;
			else 
			{
				return false;
			}
		}
		else 
		{
			count -= ret;
			sum += ret;
		}
	}
	return true;
}

void MyEvent::changeToIN() 
{
	loop_->modifyEvent(pollEdgeTrigger | pollReadAble, *this);
}

void MyEvent::changeToOUT() 
{

	loop_->modifyEvent(pollEdgeTrigger | pollWriteAble, *this);
}