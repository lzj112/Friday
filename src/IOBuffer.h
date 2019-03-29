
#ifndef IOBUFFER_H_
#define IOBUFFER_H_


#include <list>
#include <string>
#include <vector>

class IOBuffer 
{
public:
    IOBuffer();
    ~IOBuffer();
    IOBuffer(const IOBuffer& t);
    IOBuffer& operator=(const IOBuffer& t);

    bool isEmpty();
    int remainingMsg();  //还有多少可读数据
    unsigned char* messageData();
    void TryFreeMemory(); 
    void appendMsgBack(std::string& s);
    void appendMsgBack(const char* buffer);
    void appendMsgBack(unsigned char* str, int len);
    void appendMsgBack(std::vector<unsigned char>& Msgsage);
    std::vector<unsigned char> messageOnTop();


private:
    static const int MAXBUFFER = 256;
    std::list<std::vector<unsigned char> > ioBuffer;
    // std::vector<unsigned char> ioBuffer;
};


#endif