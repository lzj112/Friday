
#ifndef IOBUFFER_H_
#define IOBUFFER_H_


#include <array>
#include <vector>

#include "../base/Message.h"

class IOBuffer 
{
public:
    IOBuffer();
    ~IOBuffer();

    void append(Message&& tmp);
    void swap(IOBuffer& tmp);
    int readableBytes();
    int writableBytes();
private:
    static const int MAXBUFFER = 1024;
    int readIndex;
    int writeIndex;
    // std::vector<Message> ioBuffer;
    std::array<Message, MAXBUFFER> ioBuffer;    //循环队列
};


#endif