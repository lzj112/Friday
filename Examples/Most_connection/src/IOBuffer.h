
#ifndef IOBUFFER_H_
#define IOBUFFER_H_


#include <array>
#include <vector>

#include "Message.h"

class IOBuffer 
{
public:
    IOBuffer();
    ~IOBuffer();
    IOBuffer(const IOBuffer& t);
    IOBuffer& operator=(const IOBuffer& t);

    void appendMess(const char* buffer);
    void appendMess(Message& tmp);
    void appendMess(Message&& tmp);
    void readMess(Message& buffer);
    int readableMess()  //还有多少可读数据
    { return messCount; }
    int writableMess()  //还有多少可写空间
    { return MAXBUFFER - messCount - 1; }
    int headPoint() 
    { return readIndex; }
    bool isEmpty();
    bool isFull();


private:
    static const int MAXBUFFER = 256;
    int readIndex;
    int writeIndex;
    int messCount;
    //循环队列,牺牲一个空间来解决二义性
    std::array<Message, MAXBUFFER> ioBuffer;

};


#endif