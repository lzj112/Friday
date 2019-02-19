#ifndef MESSAGE_H_
#define MESSAGE_H_

#include <cstdint>

#include <vector>
#include <string>
#include <iostream>
 
class Message 
{
public: 
    Message();
    Message(const char* str);
    Message(std::string& str);
    Message(const Message& t);
    Message(Message&& mess) noexcept;
    Message& operator=(const Message& t);
    Message& operator=(Message&& t) noexcept; //use swap
    ~Message() {}

    void resize(int length) 
    { message_.resize(length); lengthNow = message_.size(); }
    void reserve(int length) { message_.reserve(length); }
    int length();
    void clear() { message_.clear(); }
    void releaseMessage() 
    { std::vector<char>().swap(message_); }
    std::vector<char>& message() { return message_; }
    void setType(int t) { type_ = t; }
    int type() const { return type_;}
    char* mess() { return message_.data(); } 

    void show() 
    {
        for (auto x : message_) 
            std::cout << x << std::endl;
    }

private:
    void copyMessage(const char* str, int length);

    int lengthNow;
    int type_;      //服务类型
    std::vector<char> message_;
};

#endif 