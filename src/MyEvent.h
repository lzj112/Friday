#ifndef MYEVENT_H_
#define MYEVENT_H_

#include <unistd.h>

#include <functional>
#include <sys/epoll.h>

#include "IOBuffer.h"
#include "PackageTCP.h"
#include "TimerWheel.h"
#include "EpollEventLoop.h"


//针对每个连接封装其属性和操作

class MyEvent;
class EpollEventLoop;

typedef std::function<void(void)> IOcallBack;
typedef std::function<void(void)> errorCallBack; //关闭连接直接执行,不放进任务队列
typedef std::function<void(MyEvent*, Message&)> messManage;

class MyEvent 
{
public:
    MyEvent(EpollEventLoop* loop, int fd);
    ~MyEvent() {}
    

    int fd() { return fd_; }

    void setReadCallBack(const IOcallBack& cb) 
    { readCallBack_ = cb; }
    void setWriteCallBack(const IOcallBack& cb) 
    { writeCallBack_ = cb; }
    void setCloseCallBack(const errorCallBack& cb) 
    { errorCallBack_ = cb; }
    void setMessMana(const messManage& cb) 
    { messManage_ = cb; }
    // void setEventType(int type)
    // { eventType = type; }

    void goRead();
    void goWrite();
    void goClose();

    int sendMess(Message mess);

    
private:
    bool readPackHead(PackageTCP& tmpPackage);
    bool readPackBody(PackageTCP& tmpPackage, int len);
    void appendSendBuffer(PackageTCP& tmp);
    void appendRecvBuffer(PackageTCP& tmp);
    void sendMessTo(Message tmpMess);
    bool sendMessHead(PackageTCP* pac);
    bool sendMessBody(PackageTCP* pac, int length);

    void changeToIN();
    void changeToOUT();
    

    void performMessManaCB();
    
    
    const int fd_;
    // int eventType;
    EpollEventLoop* loop_;
    void* ptr;
    
    IOBuffer sendBuffer;
    IOBuffer recvBuffer;

    IOcallBack readCallBack_;
    IOcallBack writeCallBack_;
    messManage messManage_;
    errorCallBack errorCallBack_;
};


#endif
