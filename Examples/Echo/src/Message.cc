#include <cstring>

#include "ErrLog.h"
#include "Message.h"

Message::Message() 
    : lengthNow(0),
      type_(-1)
{ message_.push_back('\0'); }

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
    type_ = t.type_;
    // std::vector<char> tmp(t.message_);
    // std::swap(message_, tmp);
    // message_.assign(t.message_.begin(), t.message_.end());
    message_ = t.message_;
    lengthNow = message_.size();
}

Message::Message(Message&& mess) noexcept
{
    std::swap(message_, mess.message());
    lengthNow = message_.size();
    type_ = mess.type();
}

Message::~Message() 
{
    lengthNow = -1;
    type_ = -1;
    {
        std::vector<char> ().swap(message_);
    }
}

Message& Message::operator=(const Message& t) 
{
    DEBUG("for sure t: ");
    t.show();
    if (this != &t) 
    {
        type_ = t.type_;
        // {
        //     std::vector<char> ().swap(message_);
        // }
        // std::vector<char> tmp(t.message_);
        // std::swap(message_, tmp);
        // message_.assign(t.message_.begin(), t.message_.end());
        message_ = t.message_;
        lengthNow = message_.size();

        // DEBUG("message=%s\n", message_.data());
    }
    return *this;
}

Message& Message::operator=(Message&& t) noexcept
{
    type_ = t.type_;
    // {
    //     std::vector<char> ().swap(message_);
    // }
    std::vector<char> tmp(t.message_);
    std::swap(message_, tmp);

DEBUG("message=%s\n", message_.data());

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

