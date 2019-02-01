
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

    void appendMess(const char* buffer, int len);
    void appendMess(Message& tmp);
    void appendMess(Message&& tmp);
    void readMess(Message& buffer);
    int readableMess()  //还有多少可读数据
    { return messCount; }
    int writableMess()  //还有多少可写空间
    { return MAXBUFFER - messCount; }
    int headPoint() 
    { return readIndex; }

private:
    static const int MAXBUFFER = 1024;
    int readIndex;
    int writeIndex;
    int messCount;
    // std::vector<Message> ioBuffer;
    std::array<Message, MAXBUFFER> ioBuffer;    //循环队列
};


#endif