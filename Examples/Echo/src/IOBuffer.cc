

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

IOBuffer::IOBuffer(const IOBuffer& t)
    : readIndex(t.readIndex),
      writeIndex(t.writeIndex),
      messCount(t.messCount)
{
    for (int i = readIndex, j = writeIndex; 
         i != ((j + 1) % MAXBUFFER); 
         (++i % MAXBUFFER))
    {
        ioBuffer[i] = t.ioBuffer[i];
    }
}

IOBuffer& IOBuffer::operator=(const IOBuffer& t) 
{
    if (this != &t) 
    {
        readIndex = t.readIndex;
        writeIndex = t.writeIndex;
        messCount = t.messCount;
        for (int i = readIndex, j = writeIndex; 
             i != ((j + 1) % MAXBUFFER); 
             (++i % MAXBUFFER))
        {
            ioBuffer[i] = t.ioBuffer[i];
        }
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
        ioBuffer[writeIndex] = tmp;
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