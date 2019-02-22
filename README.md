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

## 项目概述

项目采用 C++11 编写, 使用非阻塞 IO, 通过事件通知和回调机制执行 IO 读写, 定时器等多类事件

### Reactor函数调用流程图
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190222203150515.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl8zNjg4ODU3Nw==,size_16,color_FFFFFF,t_70)
