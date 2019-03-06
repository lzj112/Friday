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
<<<<<<< HEAD
typedef std::function<void(MyEvent*, Message&)> mesMgr;
=======
typedef std::function<void(MyEvent*, Message&)> messManage;
>>>>>>> 2dbba237396801c4e1d576a40693b98ce9c368c1

class MyEvent 
{
public:
    MyEvent(EpollEventLoop* loop, int fd);
    ~MyEvent() {}
    MyEvent(const MyEvent& t);
    
    int fd() { return fd_; }

    void setReadCallBack(const IOcallBack& cb) 
    { readCallBack_ = cb; }
    void setWriteCallBack(const IOcallBack& cb) 
    { writeCallBack_ = cb; }
    void setCloseCallBack(const errorCallBack& cb) 
    { errorCallBack_ = cb; }
<<<<<<< HEAD
    void setMesMgr(const mesMgr& cb) 
    { mesMgr_ = cb; }
=======
    void setMessMana(const messManage& cb) 
    { messManage_ = cb; }
>>>>>>> 2dbba237396801c4e1d576a40693b98ce9c368c1
    void goRead();
    void goWrite();
    void goClose();

<<<<<<< HEAD
    int sendMes(Message mess);
=======
    int sendMess(Message mess);
>>>>>>> 2dbba237396801c4e1d576a40693b98ce9c368c1
    
private:
    bool readPackHead(PackageTCP* tmpPackage);
    bool readPackBody(PackageTCP& tmpPackage, int len);
    void appendSendBuffer(PackageTCP& tmp);
    void appendRecvBuffer(PackageTCP& tmp);
    void sendMessTo(Message tmpMess);
    bool sendMessHead(PackageTCP* pac);
    bool sendMessBody(PackageTCP& pac);

    void changeToIN();
    void changeToOUT();
    

    void performMessManaCB();
    void checkForExpiration();
    
    
    const int fd_;
    EpollEventLoop* loop_;
    // int heartBeatCount;
    // TimerWheel wheel;
    // static const int INITEXPIRATION = 8;
    void* ptr;
    
<<<<<<< HEAD
    //读写buffer
=======
>>>>>>> 2dbba237396801c4e1d576a40693b98ce9c368c1
    IOBuffer sendBuffer;
    IOBuffer recvBuffer;

    IOcallBack readCallBack_;
    IOcallBack writeCallBack_;
<<<<<<< HEAD
    mesMgr mesMgr_;
=======
    messManage messManage_;
>>>>>>> 2dbba237396801c4e1d576a40693b98ce9c368c1
    errorCallBack errorCallBack_;
};


#endif
