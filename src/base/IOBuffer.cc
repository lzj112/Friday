


#include "../base/IOBuffer.h"


IOBuffer::IOBuffer() 
    : readIndex(0),
      writeIndex(0),
      messCount(0)
{}

IOBuffer::~IOBuffer() 
{

}

void IOBuffer::appendMess(const char* buffer, int len) 
{
    
}

void IOBuffer::appendMess(Message& tmp) 
{

}

void IOBuffer::appendMess(Message&) 
{

}

void IOBuffer::readMess(Message& buffer) 
{

}
