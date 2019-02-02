
#include "../net/MyEvent.h"

MyEvent::MyEvent() 
    : fd_(-1), 
      ptr(nullptr),
      readCallBack_(std::bind(&MyEvent::defRead, this)),
      writeCallBack_(std::bind(&MyEvent::defWrite, this)),
      errorCallBack_(std::bind(&MyEvent::defClose, this))
{}

MyEvent::MyEvent(int fd) 
    : fd_(fd),
      ptr(nullptr),
      readCallBack_(std::bind(&MyEvent::defRead, this)),
      writeCallBack_(std::bind(&MyEvent::defWrite, this)),
      errorCallBack_(std::bind(&MyEvent::defClose, this))
{}

void MyEvent::goRead() 
{
	PackageTCP tmpPackage;
	do 
	{
		bzero(&tmpPackage, sizeof(PackageTCP));

		
	}
}
