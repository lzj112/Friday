#ifndef MYEVENT_H_
#define MYEVENT_H_

#include <unistd.h>

#include <functional>
#include <sys/epoll.h>

#include "IOBuffer.h"
#include "PackageTCP.h"
// #include "TimerWheel.h"
// #include "TimerWheel.h"
#include "EpollEventLoop.h"


//针对每个连接封装其属性和操作

class MyEvent;
class EpollEventLoop;

typedef std::function<void(void)> IOcallBack;
typedef std::function<void(void)> errorCallBack; //关闭连接直接执行,不放进任务队列
typedef std::function<void(MyEvent*, Message&)> mesMgr;

class MyEvent 
{
public:
    MyEvent(EpollEventLoop* loop, int fd);
    ~MyEvent() 
    { 
        // if (!isClosed) ::close(fd_); 
    }
    MyEvent(const MyEvent& t);
    
    int fd() { return fd_; }

    void setReadCallBack(const IOcallBack& cb) 
    { readCallBack_ = cb; }
    void setWriteCallBack(const IOcallBack& cb) 
    { writeCallBack_ = cb; }
    void setCloseCallBack(const errorCallBack& cb) 
    { errorCallBack_ = cb; }
    void setMesMgr(const mesMgr& cb) 
    { mesMgr_ = cb; }
    void goRead();
    void goWrite();
    void goClose();

    int sendMes(Message mess);
    
private:
    bool readPackHead(PackageTCP* tmpPackage);
    bool readPackBody(PackageTCP& tmpPackage, int len);
    void appendSendBuffer(PackageTCP& tmp);
    void appendRecvBuffer(PackageTCP& tmp);
    void sendMesTo(Message tmpMess);
    bool sendMesHead(PackageTCP* pac);
    bool sendMesBody(PackageTCP& pac);

    void changeToIN();
    void changeToOUT();
    

    void performMessManaCB();
    void checkForExpiration();
    
    
    const int fd_;
    EpollEventLoop* loop_;
    bool isClosed;
    // int heartBeatCount;
    // TimerWheel wheel;
    // static const int INITEXPIRATION = 8;
    void* ptr;
    
    //读写buffer
    IOBuffer sendBuffer;
    IOBuffer recvBuffer;

    IOcallBack readCallBack_;
    IOcallBack writeCallBack_;
    mesMgr mesMgr_;
    errorCallBack errorCallBack_;
};


#endif
