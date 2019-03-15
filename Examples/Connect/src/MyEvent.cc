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
	  msgMgr_(nullptr),
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
	  msgMgr_(t.msgMgr_),
	  errorCallBack_(t.errorCallBack_)
{
	sendBuffer = t.sendBuffer;
	recvBuffer = t.recvBuffer;
}

void MyEvent::goRead() //每次读取套接字上的数据时尽可能多的读取
{
	if (readCallBack_ == nullptr)
	{
		recvBuffer.TryFreeMemory();
		int readBytes;
		readBytes = readMsgFromTCP();
		//处理拿到的数据
		if (readBytes > 0)
		{	
			performMsgsManaCB();
			changeToOUT();
		}
		else 
			loop_->delEvent(fd_);
	}
	else 
		readCallBack_();
}

int MyEvent::readMsgFromTCP() 
{
	int ret = 0, sum = 0;
	std::vector<unsigned char> tmpBuf;
	unsigned char buf[READONCELEN];
	while (1) //读取数据头
	{
		ret = ::recv(fd_, 
					 buf, 
					 READONCELEN, 
					 0);
		if (ret <= 0) 
		{
			if (errno == EINTR || 
			 	errno == EAGAIN)
				break;
			else 
			{
				::close(fd_);
				isClosed = true;
				return -1;
			}

		}
		else 
		{
			sum += ret;
			jointMessage(tmpBuf, buf, ret);
			memset(buf, READONCELEN, '\0');
		}
	}
	recvBuffer.appendMsgBack(tmpBuf);
	return sum;
}

void MyEvent::jointMessage(std::vector<unsigned char>& tmpBuf,
						   unsigned char* ctr,
						   int len) 
{
	if (ctr == nullptr || len <= 0) 
		return ;
	for (int i = 0; i < len; i++) 
		tmpBuf.push_back(*(ctr + i));
}


//读取完数据后挨个处理
//messManage_为用户指定
void MyEvent::performMsgsManaCB() 
{
	if (!recvBuffer.isEmpty()) 
	{
		if (msgMgr_ != nullptr)
		{
			msgMgr_(this, recvBuffer.messageOnTop());
		}
		else 
		{
			DEBUG("未指定处理回调\n");
		}	
	}
}

void MyEvent::goWrite() 
{
	if (writeCallBack_ != nullptr) 
		writeCallBack_();
	else if (!sendBuffer.isEmpty())
	{
		int sendBytes;;
		sendBytes = sendMsgsage();
		
		if (sendBytes == -1)
			loop_->delEvent(fd_);
	}
	changeToIN();
}


void MyEvent::sendMsgToBuffer(unsigned char* message, int len)
{
	if (message != nullptr) 
		sendBuffer.appendMsgBack(message, len);
}

void MyEvent::sendMsgToBuffer(std::vector<unsigned char>& tmpBuf) 
{
	if (!tmpBuf.empty())
		sendBuffer.appendMsgBack(tmpBuf);
}

void MyEvent::startSendPeer() 
{
	changeToOUT();
}

int MyEvent::sendMsgsage() 
{
	if (sendBuffer.isEmpty())
		return 0;
	int ret = 0, sendSum = 0; 
	std::vector<unsigned char> tmpBuf;
	while (!sendBuffer.isEmpty())
	{
		int sendOnce = 0;
		int	count = sendBuffer.remainingMsg();
		tmpBuf = sendBuffer.messageOnTop();
		while (count > sendOnce) 
		{
			ret = send(fd_,
					   tmpBuf.data() + sendOnce,
					   count,
					   0);
			if (ret <= 0) 
			{
				if (errno == EAGAIN || 
					errno == EWOULDBLOCK || 
					errno == EINTR)
					break;
				else 
				{
					::close(fd_);
					isClosed = true;
					return -1;
				}
			}
			else 
			{
				sendOnce += ret;
				sendSum += ret;
			}
		}
	}
	return sendSum;
}

void MyEvent::changeToIN() 
{
	loop_->modifyEvent(pollEdgeTrigger | pollReadAble, 
					   shared_from_this());
}

void MyEvent::changeToOUT() 
{

	loop_->modifyEvent(pollEdgeTrigger | pollWriteAble, 
					   shared_from_this());
}

// void MyEvent::checkForExpiration() 
// {
// 	if (heartBeatCount != 0) 
// 	{
		
// 	}
// 	heartBeatCount++;
// }