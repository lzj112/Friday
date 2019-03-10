#include <stdio.h>

#include "ErrLog.h"
#include "MyEvent.h"


MyEvent::MyEvent(EpollEventLoop* loop, int fd) 
    : fd_(fd),
	  loop_(loop),
	//   heartBeatCount(0),
	//   wheel(loop),
	  isClosed(false),
      ptr(nullptr),
	  readCallBack_(nullptr),
	  writeCallBack_(nullptr),
	  mesMgr_(nullptr),
	  errorCallBack_(nullptr)
{}


MyEvent::MyEvent(const MyEvent& t) 
	: fd_(t.fd_),
	  loop_(t.loop_),
	//   heartBeatCount(0),
	//   wheel(t.loop_),
	  isClosed(false),
	  ptr(t.ptr),
	  readCallBack_(t.readCallBack_),
	  writeCallBack_(t.writeCallBack_),
	  mesMgr_(t.mesMgr_),
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
		int isEndRead;
		do 
		{
			bzero(&tmpBuffer, sizeof(PackageTCP));
			isEndRead = readPackHead(&tmpBuffer);
			if (isEndRead == 1)
				isEndRead = readPackBody(tmpBuffer, tmpBuffer.head.length);
			if (isEndRead == 1) 
			{
				// heartBeatCount = 0;	//心跳计数清零
				appendRecvBuffer(tmpBuffer);
			}
		}	while (isEndRead == 1 && !recvBuffer.isFull());


		//处理拿到的数据
		if (isEndRead != -1)
			performMessManaCB();
		else 
			loop_->delEvent(fd_);
	}
	else 
		readCallBack_();
}

int MyEvent::readPackHead(PackageTCP* pack) 
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
		{
			if (errno == EINTR || 
			 	errno == EAGAIN)
				return 0;
			else 
			{
				::close(fd_);
				isClosed = true;
				isRecvHeadOK = false;
				return -1;
			}

		}
		else 
			sum += ret;
	}

	return 1;
}

int MyEvent::readPackBody(PackageTCP& tmp, int len) 
{
	int count = len, ret = 0, sum = 0;
	bool isRecvBodyOK = true;
	while (count > 0) 
	{
		ret = ::recv(fd_, 
					 (tmp.body + sum), 
					 MAXBODY,
					 0);
		if (ret <= 0) 
		{
			if (errno == EINTR || 
			 	errno == EAGAIN)
				 return 0;
			else 
			{
				::close(fd_);
				isClosed = true;
				isRecvBodyOK = false;
				return -1;
			}

		}
		else
		{
			count -= ret;
			sum += ret;
		}
	}
	return 1;
}

void MyEvent::appendRecvBuffer(PackageTCP& tmp)
{
	Message tmpMess(tmp.body);
	tmpMess.setType(tmp.head.type);
	recvBuffer.appendMes(std::move(tmpMess));
	// recvBuffer.appendMes(tmpMess);
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
			recvBuffer.readMes(tmpMess);
			if (mesMgr_ != nullptr)
			{
				mesMgr_(this, tmpMess);
			}
			else 
			{
				std::cout << "take the message:" << std::endl;
				tmpMess.show();
			}
		}	while (!recvBuffer.isEmpty());

		changeToOUT();
	}
}

void MyEvent::goWrite() 
{
	if (writeCallBack_ != nullptr) 
		writeCallBack_();
	else if (!sendBuffer.isEmpty()) 
	{
		Message tmpMess;
		int isSucSend;
		do 
		{
			memset(&tmpMess, 0, sizeof(Message));
			sendBuffer.readMes(tmpMess);
			isSucSend = sendMesTo(tmpMess);
		}	while (!sendBuffer.isEmpty() && isSucSend == 1);
		// 改为监听可读事件
		if (isSucSend != -1)
			changeToIN();
		else 
			loop_->delEvent(fd_);
	}
}


int MyEvent::sendMes(Message mes)
{
	sendBuffer.appendMes(std::move(mes));	//加入写buffer
	// sendBuffer.appendMes(mes);	//加入写buffer

	// changeToOUT();
}

//重新添加 注册可写
int MyEvent::sendMesTo(Message tmpMes) 
{
	PackageTCP package(tmpMes.type(), tmpMes.mes());
	
	int isSucSend = sendMesHead(&package);
	if (isSucSend == 1)
		isSucSend = sendMesBody(package);
	return isSucSend;
}

int MyEvent::sendMesHead(PackageTCP* pac) 
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
				return 0;
			else 
			{
				::close(fd_);
				isClosed = true;
				return -1;
			}
		}
		else 
		{
			count -= ret;
			sum += ret;
		}
	}
	return 1;
}

int MyEvent::sendMesBody(PackageTCP& pac) 
{
	int count = pac.head.length, ret = 0, sum = 0;
	while (count > 0) 
	{
		ret = send(fd_,
				   (pac.body + sum),
				   MAXBODY,
				   0);
		if (ret <= 0) 
		{
			if (errno == EAGAIN || 
				errno == EWOULDBLOCK || 
				errno == EINTR)
				return 0;
			else 
			{
				::close(fd_);
				isClosed = true;
				return -1;
			}
		}
		else 
		{
			count -= ret;
			sum += ret;
		}
	}
	return 1;
}

void MyEvent::changeToIN() 
{
	loop_->modifyEvent(pollEdgeTrigger | pollReadAble, *this);
}

void MyEvent::changeToOUT() 
{

	loop_->modifyEvent(pollEdgeTrigger | pollWriteAble, *this);
}

// void MyEvent::checkForExpiration() 
// {
// 	if (heartBeatCount != 0) 
// 	{
		
// 	}
// 	heartBeatCount++;
// }