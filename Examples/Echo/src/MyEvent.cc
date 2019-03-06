#include <stdio.h>

<<<<<<< HEAD
#include "ErrLog.h"
=======
>>>>>>> 2dbba237396801c4e1d576a40693b98ce9c368c1
#include "MyEvent.h"


MyEvent::MyEvent(EpollEventLoop* loop, int fd) 
    : fd_(fd),
	  loop_(loop),
	//   heartBeatCount(0),
	//   wheel(loop),
      ptr(nullptr),
	  readCallBack_(nullptr),
	  writeCallBack_(nullptr),
<<<<<<< HEAD
	  mesMgr_(nullptr),
=======
	  messManage_(nullptr),
>>>>>>> 2dbba237396801c4e1d576a40693b98ce9c368c1
	  errorCallBack_(nullptr)
{}


MyEvent::MyEvent(const MyEvent& t) 
	: fd_(t.fd_),
	  loop_(t.loop_),
	//   heartBeatCount(0),
	//   wheel(t.loop_),
	  ptr(t.ptr),
	  readCallBack_(t.readCallBack_),
	  writeCallBack_(t.writeCallBack_),
<<<<<<< HEAD
	  mesMgr_(t.mesMgr_),
=======
	  messManage_(t.messManage_),
>>>>>>> 2dbba237396801c4e1d576a40693b98ce9c368c1
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
<<<<<<< HEAD
				DEBUG("recv data = %s\n", tmpBuffer.body);
=======
>>>>>>> 2dbba237396801c4e1d576a40693b98ce9c368c1
				// heartBeatCount = 0;	//心跳计数清零
				appendRecvBuffer(tmpBuffer);
			}
		}	while (isEndRead == true);

<<<<<<< HEAD
		DEBUG("结束一次循环recv\n");
=======
>>>>>>> 2dbba237396801c4e1d576a40693b98ce9c368c1
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
		{
			if (errno == EINTR || 
			 	errno == EAGAIN)
				return false;
			else 
			{
				printf("%d断开连接\n", fd_);
				::close(fd_);
				isRecvHeadOK = false;
				return false;
			}

		}
		else 
			sum += ret;
	}

	return true;
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
		if (ret <= 0) 
		{
			if (errno == EINTR || 
			 	errno == EAGAIN)
				 return false;
			else 
			{
				printf("断开连接\n");
				::close(fd_);
				isRecvBodyOK = false;
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

void MyEvent::appendRecvBuffer(PackageTCP& tmp)
{
	Message tmpMess(tmp.body);
	tmpMess.setType(tmp.head.type);
<<<<<<< HEAD
	// recvBuffer.appendMess(std::move(tmpMess));
	recvBuffer.appendMes(tmpMess);
}


=======
	recvBuffer.appendMess(std::move(tmpMess));
}

>>>>>>> 2dbba237396801c4e1d576a40693b98ce9c368c1
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
<<<<<<< HEAD
			recvBuffer.readMes(tmpMess);
			if (mesMgr_ != nullptr)
			{
				mesMgr_(this, tmpMess);
=======
			recvBuffer.readMess(tmpMess);
			if (messManage_ != nullptr)
			{
				messManage_(this, tmpMess);
>>>>>>> 2dbba237396801c4e1d576a40693b98ce9c368c1
			}
			else 
			{
				std::cout << "take the message:" << std::endl;
				tmpMess.show();
			}
		}	while (!recvBuffer.isEmpty());
<<<<<<< HEAD

		changeToOUT();
=======
>>>>>>> 2dbba237396801c4e1d576a40693b98ce9c368c1
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
<<<<<<< HEAD
			sendBuffer.readMes(tmpMess);
=======
			sendBuffer.readMess(tmpMess);
>>>>>>> 2dbba237396801c4e1d576a40693b98ce9c368c1
			sendMessTo(tmpMess);
		}	while (!sendBuffer.isEmpty());
		// 改为监听可读事件
		changeToIN();
	}
}


<<<<<<< HEAD
int MyEvent::sendMes(Message mes)
{
	DEBUG("here is function sendMes\n");
	// sendBuffer.appendMess(std::move(mes));	//加入写buffer
	sendBuffer.appendMes(mes);	//加入写buffer

	// changeToOUT();
=======
int MyEvent::sendMess(Message mess)
{
	sendBuffer.appendMess(std::move(mess));	//加入写buffer

	changeToOUT();
>>>>>>> 2dbba237396801c4e1d576a40693b98ce9c368c1
}

//重新添加 注册可写
void MyEvent::sendMessTo(Message tmpMess) 
{
	PackageTCP package;
<<<<<<< HEAD
	if (tmpMess.mes() != nullptr)
    	strcpy(package.body, tmpMess.mes());
=======
    strcpy(package.body, tmpMess.mess());
>>>>>>> 2dbba237396801c4e1d576a40693b98ce9c368c1
    package.head.type = tmpMess.type();
    package.head.length = sizeof(package.body);
	
	bool isSucSend = sendMessHead(&package);
	if (isSucSend)
		isSucSend = sendMessBody(package);

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
				printf("断开连接\n");
				::close(fd_);
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

bool MyEvent::sendMessBody(PackageTCP& pac) 
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
				continue;
			else 
			{
				printf("断开连接\n");
				::close(fd_);
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

// void MyEvent::checkForExpiration() 
// {
// 	if (heartBeatCount != 0) 
// 	{
		
// 	}
// 	heartBeatCount++;
// }