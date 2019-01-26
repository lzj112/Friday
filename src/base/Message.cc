#include <cstring>

#include "../base/Message.h"

Message::Message(const char* str) : lengthNow(0)
{  
    int ctrLength = strlen(str) + 1;
    copy(str, ctrLength);
}

Message::Message(std::string& str) : lengthNow(0) 
{
    const char* ctr = str.c_str();
    int ctrLength = strlen(ctr) + 1;
    copy(ctr, ctrLength);
}

Message::Message(Message&& mess) noexcept
{

}

void Message::copy(const char* ctr, int length) 
{
    //有太多闲置空间
    if (lengthNow > (length * 2 + 1))
    {
        message.resize(length);
    }
    else 
    {
        message.reserve(length);
    }

    message.clear();
    for(int i = 0; i < length; i++) 
    {
        message.push_back(*(ctr + i));
    }
    
    lengthNow = message.size();
}

