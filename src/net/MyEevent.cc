
#include "../net/MyEvent.h"

MyEvent::MyEvent() 
    : fd_(-1), 
      ptr(nullptr),
      readCallBack_(std::bind(&MyEvent::defRead, 
                              this, 
                              std::placeholders::_1)),
      writeCallBack_(std::bind(&MyEvent::defWrite, 
                               this,
                               std::placeholders::_1)),
      closeCallBack_(std::bind(&MyEvent::defClose, 
                               this,
                               std::placeholders::_1))
{

}

MyEvent::MyEvent(int fd) 
    : fd_(fd),
      ptr(nullptr),
      readCallBack_(std::bind(&MyEvent::defRead, 
                              this, 
                              std::placeholders::_1)),
      writeCallBack_(std::bind(&MyEvent::defWrite, 
                               this,
                               std::placeholders::_1)),
      closeCallBack_(std::bind(&MyEvent::defClose, 
                               this,
                               std::placeholders::_1))
{
    
}
