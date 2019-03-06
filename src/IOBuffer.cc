

#include <utility>

<<<<<<< HEAD
#include "ErrLog.h"
=======
>>>>>>> 2dbba237396801c4e1d576a40693b98ce9c368c1
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
<<<<<<< HEAD
      messCount(t.messCount),
      ioBuffer(t.ioBuffer)
{}
=======
      messCount(t.messCount)
{
    for (int i = readIndex, j = writeIndex; 
         i != ((j + 1) % MAXBUFFER); 
         (++i % MAXBUFFER))
    {
        ioBuffer[i] = t.ioBuffer[i];
    }
}
>>>>>>> 2dbba237396801c4e1d576a40693b98ce9c368c1

IOBuffer& IOBuffer::operator=(const IOBuffer& t) 
{
    if (this != &t) 
    {
        readIndex = t.readIndex;
        writeIndex = t.writeIndex;
        messCount = t.messCount;
<<<<<<< HEAD
        ioBuffer = t.ioBuffer;
=======
        for (int i = readIndex, j = writeIndex; 
             i != ((j + 1) % MAXBUFFER); 
             (++i % MAXBUFFER))
        {
            ioBuffer[i] = t.ioBuffer[i];
        }
>>>>>>> 2dbba237396801c4e1d576a40693b98ce9c368c1
    }
}



<<<<<<< HEAD
void IOBuffer::appendMes(const char* buffer) 
=======
void IOBuffer::appendMess(const char* buffer) 
>>>>>>> 2dbba237396801c4e1d576a40693b98ce9c368c1
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

<<<<<<< HEAD
void IOBuffer::appendMes(Message& tmp) 
=======
void IOBuffer::appendMess(Message& tmp) 
>>>>>>> 2dbba237396801c4e1d576a40693b98ce9c368c1
{
    if (!isFull()) 
    {
        ioBuffer[writeIndex] = tmp;
        writeIndex = ++writeIndex % MAXBUFFER;
    }
}

<<<<<<< HEAD
void IOBuffer::appendMes(Message&& tmp) 
=======
void IOBuffer::appendMess(Message&& tmp) 
>>>>>>> 2dbba237396801c4e1d576a40693b98ce9c368c1
{
    if (!isFull()) 
    {
        ioBuffer[writeIndex] = std::forward<Message&&> (tmp);
        writeIndex = ++writeIndex % MAXBUFFER;
<<<<<<< HEAD

    }
}

void IOBuffer::readMes(Message& buffer) 
=======
    }
}

void IOBuffer::readMess(Message& buffer) 
>>>>>>> 2dbba237396801c4e1d576a40693b98ce9c368c1
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