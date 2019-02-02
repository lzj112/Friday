#ifndef MYEVENT_H_
#define MYEVENT_H_

#include <unistd.h>

#include <functional>
#include <sys/epoll.h>

#include "../base/IOBuffer.h"
#include "../base/TimerWheel.h"

//IOCallBack是接收完数据的逻辑处理(也是由用户指定的)
//接收数据和发送数据都是一样的
typedef std::function<int(void)> IOcallBack;
typedef std::function<void(void)> errorCallBack; //关闭连接直接执行,不放进任务队列

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
    MyEvent();
    MyEvent(int fd);
    ~MyEvent() {}
    
    void setFd(int fd) { fd_ = fd; }

    int fd() { return fd_; }

    void setReadCallBack(const IOcallBack& cb) 
    { readCallBack_ = cb; }
    void setWriteCallBack(const IOcallBack& cb) 
    { writeCallBack_ = cb; }
    void setCloseCallBack(const errorCallBack& cb) 
    { errorCallBack_ = cb; }

    int defRead();
    int defWrite();
    void defClose() 
    {
        //log TODO
        ::close(fd_);
        fd_ = -1;
    }

    void goRead();
    void goWrite();
    void goClose();
    // void goRead() { readCallBack_(); }
    // void goWrite() { writeCallBack_(); }
    // void goClose() { errorCallBack_(); }
    
private:
    int fd_;
    void* ptr;
    
    IOBuffer socketBuffer;

    IOcallBack readCallBack_;
    IOcallBack writeCallBack_;
    errorCallBack errorCallBack_;
};

#endif
