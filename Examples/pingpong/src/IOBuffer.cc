
#include <cstring>
#include <utility>

#include <algorithm>

#include "ErrLog.h"
#include "IOBuffer.h"


IOBuffer::IOBuffer() 
{}

IOBuffer::~IOBuffer() 
{
    std::list<std::vector<unsigned char> > ().swap(ioBuffer);
}

IOBuffer::IOBuffer(const IOBuffer& t)
    : ioBuffer(t.ioBuffer)
{}

IOBuffer& IOBuffer::operator=(const IOBuffer& t) 
{
    if (this != &t) 
        ioBuffer = t.ioBuffer;
}

void IOBuffer::TryFreeMemory() 
{
    if (!ioBuffer.empty()) 
        ioBuffer.clear();
}

std::vector<unsigned char> IOBuffer::messageOnTop() 
{
    auto it = ioBuffer.front();
    ioBuffer.pop_front();
    return it;
}

unsigned char* IOBuffer::messageData() 
{
    return ioBuffer.front().data();
}

int IOBuffer::remainingMsg() 
{
    return ioBuffer.front().size();
}

void IOBuffer::appendMsgBack(const char* buffer) 
{
    if (buffer != nullptr) 
    {
        std::vector<unsigned char> tmpBuf;
        for (int i = 0; i < strlen(buffer) + 1; i++)
            tmpBuf.push_back(*(buffer + i));
        ioBuffer.emplace_back(tmpBuf);
    }   
}

void IOBuffer::appendMsgBack(std::string& s) 
{
    if (!s.empty())
    {
        std::vector<unsigned char> tmpBuf;
        for_each(s.begin(), 
                 s.end(), 
                 [&tmpBuf](char c){
            tmpBuf.push_back(c);
        });
        ioBuffer.emplace_back(tmpBuf);
    }
}

void IOBuffer::appendMsgBack(unsigned char* str, int len) 
{
    if (str != nullptr && len > 0) 
    {
        std::vector<unsigned char> tmpBuf;
        for (int i = 0; i < len; i++)
            tmpBuf.push_back(*(str + i));
        ioBuffer.emplace_back(tmpBuf);
    }
}

void IOBuffer::appendMsgBack(std::vector<unsigned char>& message) 
{
    ioBuffer.emplace_back(message);
}

bool IOBuffer::isEmpty() 
{
    return ioBuffer.empty();
}