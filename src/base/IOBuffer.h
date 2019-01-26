
#ifndef IOBUFFER_H_
#define IOBUFFER_H_


#include <vector>

#include "../base/Message.h"

class IOBuffer 
{
public:
    IOBuffer();
    ~IOBuffer();

    void append(Message&& a);
private:
    
    int readIndex;
    int writeIndex;
    std::vector<Message> ioBuffer;
};


#endif