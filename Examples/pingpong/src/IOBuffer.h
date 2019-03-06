
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

    void appendMes(const char* buffer);
    void appendMes(Message& tmp);
    void appendMes(Message&& tmp);
    void readMes(Message& buffer);
    int readableMes()  //还有多少可读数据
    { return messCount; }
    int writableMes()  //还有多少可写空间
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