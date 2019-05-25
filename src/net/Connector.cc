
#include <signal.h>

#include <functional>

#include "ErrLog.h"
#include "Connector.h"

Connector::Connector(EpollEventLoop* baseLoop,
                     SocketTCP* sock,
                     const char* ip,
                     int port)
    : loop_(baseLoop),
      cliSock(sock),
      reTryDelay(initReTryDelay),
      serAddr(ip, port),
      timerContainer(baseLoop)
{}

void Connector::connect() 
{
    int ret = cliSock->connect(serAddr);
    int saveErrno = (ret == 0) ? ret : errno;
    switch (saveErrno) 
    {
        case 0:             //连接成功
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

//连接立刻成功,返回
void Connector::connSuccessful() 
{
    connSucced_(cliSock->fd());
    loop_->stopLoop();
}

//正在连接
void Connector::inConnection() 
{
    std::shared_ptr<MyEvent> ev = 
        std::make_shared<MyEvent> (loop_, cliSock->fd());
    ev->setWriteCallBack(std::bind(&Connector::isConnOk, this));
    //注册可写事件
    loop_->regWriteable(ev);
}

void Connector::gotError() 
{
    //log TODO
    int error = cliSock->getSocketState();

    reConnect();
}


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
            DEBUG("是自连接\n");
            reConnect();
        }
        else
        {
            DEBUG("成功连接\n");
            connSuccessful();
        }
    }
    else 
    {
        DEBUG("连接失败,重连\n");
        //不是EISCONN就是连接失败
        reConnect();
    }
}

void Connector::reConnect() 
{
    if (reTryDelay < maxCEILING) 
    {
        cliSock->reSet();
        timerContainer.addTimer(reTryDelay,
                                0,
                                std::bind(&Connector::connect, this));
        reTryDelay = 2 * reTryDelay;
    }   
    else
        DEBUG("无法连接\n");        
}
