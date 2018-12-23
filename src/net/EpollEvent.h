#ifndef EPOLLEVENT_H_
#define EPOLLEVENT_H_

#include <sys/epoll.h>

#include <map>
#include <vector>
#include <memory>
#include <functional>

#include "../base/FileDes.h"
#include "../net/EpollBase.h"

class EpollEvent 
{
public:
    EpollEvent();
    EpollEvent(FileDes fd_);    //server传入监听套接字
    ~EpollEvent();

    void handleEvent();
    void setReadCallBack(IOcallBack& cb) { readCallBack_ = cb; }
    void setWriteCallBack(IOcallBack& cb) { writeCallBack_ = cb; }
    void setCloseCallBack(closeCallBack& cb) { closeCallBack_ = cb; }

    void delEvents();   //将fd从epoll事件合集删除
    // void updateEvent(); //将fd添加到epoll事件合集
    void updateReadEvents();    //注册可读事件到epoll事件合集
    void updateWriteEvents();   //注册可写事件到epoll事件合集

    void poll(std::vector<epoll_event>& events, int timeout);

private:
    std::unique_ptr<EpollBase> epollFd;

    IOcallBack readCallBack_;
    IOcallBack writeCallBack_;
    closeCallBack closeCallBack_;
};

#endif