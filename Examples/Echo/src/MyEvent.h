#ifndef MYEVENT_H_
#define MYEVENT_H_

#include <unistd.h>
#include <sys/epoll.h>

#include <memory>
#include <functional>

#include "IOBuffer.h"
// #include "TimerWheel.h"
// #include "TimerWheel.h"
#include "EpollEventLoop.h"


//针对每个连接封装其属性和操作

class MyEvent;
class EpollEventLoop;

typedef std::function<void(void)> IOcallBack;
typedef std::function<void(void)> errorCallBack; //关闭连接直接执行,不放进任务队列
typedef std::function<void(MyEvent*, std::vector<unsigned char>)> msgMgr;

class MyEvent : public std::enable_shared_from_this<MyEvent>
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
    void setMsgMgr(const msgMgr& cb) 
    { msgMgr_ = cb; }
    void goRead();
    void goWrite();
    void goClose();

    void sendMsgToBuffer(unsigned char* msgsage, int len);
    void sendMsgToBuffer(std::vector<unsigned char>& tmpBuf);
    void startSendPeer();
    
private:
    int readMsgFromTCP();
    int sendMsgsage();
    void changeToIN();
    void changeToOUT();
    

    void performMsgsManaCB();
    void jointMessage(std::vector<unsigned char>& tmpBuf,
                      unsigned char* ctr,
                      int len);
    // void checkForExpiration();
    
    
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
    msgMgr msgMgr_;
    errorCallBack errorCallBack_;
};


#endif
