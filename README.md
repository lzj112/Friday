# Friday 网络库

---

## 功能

- 高并发
- 高性能
- 只支持 TCP
- 支持优雅退出
- 服务端心跳保活 (TODO)
- 时间轮定时器 timerfd
- 服务端有读写 BUFFER 
- 客户端提供了 Connector 重连功能


>认为会是两台采用相同字节序的主机使用该网络库通信, 所以并没有提供字节序转换功能

---

# 项目概述

项目采用 C++11 编写, 使用非阻塞 IO, 通过事件通知和回调机制执行 IO 读写, 定时器等多类事件

## Reactor函数调用流程图
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190222203150515.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl8zNjg4ODU3Nw==,size_16,color_FFFFFF,t_70)

# 部分技术点实现

## Reactor 的实现
```
void EpollEventLoop::loop() 
{
    isLooping = true;
    while (isLooping) 
    {
        epoll_.wait(events, EPOLLWAITFOR);
        handleEvents();
        {
            std::vector<epoll_event> evs(events.size());
            std::swap(events, evs);
        }
    }

    isEnd = true;
}
```
`EpollEventLoop::loop()`中循环, 调用`EpollBase::wait()`返回就绪事件集合, 调用`EpollEventLoop::handleEvents() `分类事件, 执行`MyEvent`中的对应回调函数
## 服务端 accept 
- `FServer`中仍然采用的是循环`accept`的方法, 即: 
```
	while (1) 
    {
        memset(&peer, 0, peerLen);	//peer为sockaddr_in类型
        connfd = serverFd->accept(&peer);
        if (connfd < 0) 
            break;
        else 
        {
          //...
            printf("got a new connection fd = %d\n", connfd);
        }
    }
```
如果同一时间有大量连接, 内核中的`accept`队列可能很快填满, 一次只 accept 一个效率不如这样做, 经过测试这样确实更快一些

还有一个制约同一时刻的最大连接数的因素应该就是`backlog`的值了, 我这里只是指定为`10`

-  accept函数:
```
int SocketTCP::accept(sockaddr_in* peer)
{
    socklen_t peerAddrLen = sizeof(sockaddr_in);

    errno = 0;
    int ret = ::accept(sockfd, 
                      (sockaddr *)peer, 
                      &peerAddrLen);
    if (ret > 0)
    {
        int old_option = fcntl(ret, F_GETFL, 0);
        int new_option = old_option | O_NONBLOCK;
        fcntl(ret, F_SETFL, new_option);
        return ret;
    }
    else 
        return ret;
    // return ::accept(socketFd->fd(), 
    //                   (sockaddr *)peer, 
    //                   &peerAddrLen);
}
```
因为`Friday`采取非阻塞 IO, 而我发现监听 socket 是非阻塞的, 但是连接建立后返回的那个代表新客户端连接的文件描述符仍是阻塞的

## 自动重连,非阻塞connect

阻塞不必说, 非阻塞`connect`就要涉及到判断连接是否建立成功
`Friday`中选择的做法是连续调用`::connect`函数, 在连接已经建立的情况下, 再次调用`connect`会返回错误`EISCONN`

大概流程是客户端调用`Connecor::connect`发起连接, 然后注册该 socket 监听可写事件, 当连接成功建立会触发可写事件, 这时再次调用`::connect`函数判断返回结果 
>连接建立错误时会触发可读可写, 所以需要二次判断

```
void Connector::isConnOk() 
{
    int ret = cliSock->connect(serAddr);
    int error = (ret == 0) ? ret : errno;
    if (error == 0)
    {
        connSuccessful();
    }
    else if (error == EISCONN) 
    {
        //是自连接
        if (cliSock->isSelfConnection())
        {
            printf("是自连接\n");
            reConnect();
        }
        else
        {
            printf("成功连接\n");
            connSuccessful();
        }
    }
    else 
    {
        printf("连接失败,重连\n");
        //不是EISCONN就是连接失败
        reConnect();
    }
}
```
## 心跳 TODO
- 思路
每个事件存储成员`count`, 添加定时器, 每隔一段时间 count++, 有数据到来时 count 清零, 当 count 到达三次时, 断开连接
使用时间轮增加效率

## IOBuffer
应用层 buffer 必不可少, `IOBuffer`是一个循环队列, 内里存储的是固定`Message`类型的数据
`MyEvent`的回调函数读取到数据时, 存入`recvBuffer`, 然后处理.

我采取每次循环读取数据, 直到这次没有数据可读或者是`recvBuffer`已满, 然后处理完数据, 发送数据同理, 先将要发送的数据存入`sendBuffer`, 然后一次发完, 因为我认为不应该有数据堆积, 有了就要尽快处理

## 时间轮
时间轮我采用 set 的自排序功能 `std::vector<std::set<Timer> > wheel` 实现其中定时器的排序
而`set map`遵循严格弱序化, 即:
```
A > B 和 B > A 不能同时为真 
```
STL 容器判断等价:

 `!(A < B) && !(B < A) 成立时为等价`