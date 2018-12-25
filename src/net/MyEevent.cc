
#include "../net/MyEvent.h"

MyEvent::MyEvent() 
    : fd_(-1), 
      timerfd_(-1),
      ptr(nullptr),
      readCallBack_(std::bind(&MyEvent::defRead, this)),
      writeCallBack_(std::bind(&MyEvent::defWrite, this)),
      closeCallBack_(std::bind(&MyEvent::defClose, this))
{

}

MyEvent::MyEvent(int fd, int timeout) 
    : fd_(fd),
      timerfd_(timeout),
      ptr(nullptr),
      readCallBack_(std::bind(&MyEvent::defRead, this)),
      writeCallBack_(std::bind(&MyEvent::defWrite, this)),
      closeCallBack_(std::bind(&MyEvent::defClose, this))
{
    
}
