

#include <utility>

#include "IOBuffer.h"


IOBuffer::IOBuffer() 
    : readIndex(0),
      writeIndex(0),
      messCount(0)
{}

IOBuffer::~IOBuffer() 
{
    {
        std::array<Message, MAXBUFFER> ().swap(ioBuffer);
    }
}

void IOBuffer::appendMess(const char* buffer) 
{
    if (buffer != nullptr) 
    {
        if (!isFull()) 
        {
            Message tmp(buffer);
            ioBuffer[writeIndex] = std::move(tmp);
            writeIndex = ++writeIndex % MAXBUFFER;
        }
    }
}

void IOBuffer::appendMess(Message& tmp) 
{
    if (!isFull()) 
    {
        printf("iobuffer中存入数据前tmp.mess=%s\n", tmp.mess());
        ioBuffer[writeIndex] = tmp;
        printf("iobuffer中存入数据=%s\n", ioBuffer[writeIndex]);
        writeIndex = ++writeIndex % MAXBUFFER;
    }
}

void IOBuffer::appendMess(Message&& tmp) 
{
    if (!isFull()) 
    {
        ioBuffer[writeIndex] = std::forward<Message&&> (tmp);
        writeIndex = ++writeIndex % MAXBUFFER;
    }
}

void IOBuffer::readMess(Message& buffer) 
{
    if (!isEmpty()) 
    {
        buffer = ioBuffer[readIndex];
        printf("iobuffer中读取到数据%s\n", buffer.mess());
        readIndex = ++readIndex % MAXBUFFER;
    }
}

bool IOBuffer::isEmpty() 
{
    if (readIndex == writeIndex &&
        messCount == 0)
        return true;
    else 
        return false;
}

bool IOBuffer::isFull() 
{
    if (messCount == (MAXBUFFER - 1) && 
        readIndex == (writeIndex + 1) % MAXBUFFER)
        return true;
    else 
        return false;
}