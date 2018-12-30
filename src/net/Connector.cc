
#include "../net/Connector.h"

Connector::Connector(EpollEventLoop* baseLoop,
                     SocketTCP* p,
                     InitSockAddr* addr)
    : loop_(baseLoop),
      cliSock(p),
      serAddr(addr)
{

}


void Connector::connect() 
{
    int ret = cliSock->connect(*serAddr);
    switch (ret) 
    {
        case 0:             //连接成功
            connSuccessful();
            break;
        case EINTR:         //被信号中止
        case EINPROGRESS:   //正在尝试连接
            inConnection();
            break;
        
        case EAGAIN:        //再试一次,对于connect是错误
        case EADDRINUSE:    //本地地址处于使用状态
        case ENETUNREACH:   //网络不可达
        case ECONNREFUSED:  //拒绝连接
        case EADDRNOTAVAIL: //地址分配错误
            reConnect();
            break;

        case EPERM:         //用户在套接字广播标志未设
        case EBADF:         //非法文件描述符
        case EACCES:        //置时尝试连接从而导致失败
        case EFAULT:        //指向套接字结构体的地址非法
        case EALREADY:      //原来的请求为完成
        case ENOTSOCK:      //文件描述符不与套接字相关
        case EAFNOSUPPORT:  //参数serv_add地址非法
            //log TODO
            std::cout << "connect error,close fd" << std::endl;
            cliSock->close();
            break;
        default:
            //log TODO
            std::cout << "Unexpexted error" <<std::endl;
            cliSock->close();
            break;
    }   
}

void Connector::inConnection() 
{
    MyEvent ev(cliSock->fd(), -1);
    ev.setWriteCallBack(std::bind(&Connector::isConnOk, 
                                  this,
                                  std::placeholders::_1));
    loop_->regReadable(ev);
}