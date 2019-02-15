#include <cstring>

#include "../base/Message.h"

Message::Message() 
    : lengthNow(0),
      type_(-1)
{}

Message::Message(const char* str) : lengthNow(0), type_(-1)
{  
    int ctrLength = strlen(str) + 1;
    copyMessage(str, ctrLength);
}

Message::Message(std::string& str) : lengthNow(0), type_(-1)
{
    const char* ctr = str.c_str();
    int ctrLength = strlen(ctr) + 1;
    copyMessage(ctr, ctrLength);
}

Message::Message(const Message& t) 
{
    int len = t.message_.size();
    for (int i = 0; i < len; i++)
        message_.push_back(*(t.message_.data() + i));
    lengthNow = message_.size();
    type_ = t.type_;
}

Message::Message(Message&& mess) noexcept
{
    std::swap(mess.message(), message_);
    lengthNow = message_.size();
    type_ = mess.type();
}

Message& Message::operator=(const Message& t) 
{
    if (this != &t) 
    {
        type_ = t.type_;
        {
            std::vector<char> ().swap(message_);
        }
        int len = t.message_.size();
        for (int i = 0; i < len; i++)
            message_.push_back(*(t.message_.data() + i));
        lengthNow = message_.size();
    }
    return *this;
}

Message& Message::operator=(Message&& t) noexcept
{
    type_ = t.type_;
    {
        std::vector<char> ().swap(message_);
    }
    int len = t.message_.size();
    for (int i = 0; i < len; i++)
        message_.push_back(*(t.message_.data() + i));
    lengthNow = message_.size();

    return *this;
}

int Message::length() 
{
    return (lengthNow == message_.size()) ? lengthNow : message_.size(); 
}

void Message::copyMessage(const char* ctr, int length) 
{
    //有太多闲置空间
    if (lengthNow > (length * 2 + 1))
    {
        message_.resize(length);
    }
    else 
    {
        message_.reserve(length);
    }

    message_.clear();
    for(int i = 0; i < length; i++) 
    {
        message_.push_back(*(ctr + i));
    }
    
    lengthNow = message_.size();
}

