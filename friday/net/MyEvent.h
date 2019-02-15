#ifndef MYEVENT_H_
#define MYEVENT_H_

#include <unistd.h>

#include <functional>
#include <sys/epoll.h>

#include "../base/IOBuffer.h"
#include "../base/PackageTCP.h"
#include "../base/TimerWheel.h"
#include "../net/EpollEventLoop.h"

typedef std::function<int(void)> IOcallBack;
typedef std::function<void(void)> errorCallBack; //关闭连接直接执行,不放进任务队列
typedef std::function<void(const MyEvent*, const Message&)> messManage;

enum EPOLLEVENTS
{
    pollPri = EPOLLPRI,
    pollErr = EPOLLERR,
    pollLevelTrigger = 0,
    pollHangUp = EPOLLHUP,
    pollReadAble = EPOLLIN,
    pollWriteAble = EPOLLOUT,
    pollEdgeTrigger = EPOLLET,
    pollRDHangUp = EPOLLRDHUP,
    pollOneShot = EPOLLONESHOT,
};

//针对每个连接封装其属性和操作

class MyEvent 
{
public:
    MyEvent(EpollEventLoop* loop, int fd);
    ~MyEvent() {}
    

    int fd() { return fd_; }

    // void setReadCallBack(const IOcallBack& cb) 
    // { readCallBack_ = cb; }
    void setWriteCallBack(const IOcallBack& cb) 
    { writeCallBack_ = cb; }
    void setCloseCallBack(const errorCallBack& cb) 
    { errorCallBack_ = cb; }
    void setMessMana(const messManage& cb) 
    { messManage_ = cb; }

    void goRead();
    void goWrite();
    void goClose();

    int sendMess(PackageTCP& tmpPack);

    
private:
    bool readPackHead(PackageTCP& tmpPackage);
    bool readPackBody(PackageTCP& tmpPackage, int len);
    void appendSendBuffer(PackageTCP& tmp);
    void appendRecvBuffer(PackageTCP& tmp);
    void sendMess(Message tmpMess);

    void changeToIN();
    void changeToOUT();
    

    void performMessManaCB();
    
    
    const int fd_;
    int eventType;
    EpollEventLoop* loop_;
    void* ptr;
    
    IOBuffer sendBuffer;
    IOBuffer recvBuffer;

    // IOcallBack readCallBack_;
    IOcallBack writeCallBack_;
    messManage messManage_;
    errorCallBack errorCallBack_;
};

#endif
