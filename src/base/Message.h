#ifndef MESSAGE_H_
#define MESSAGE_H_

#include <cstdint>

#include <vector>
#include <string>
#include <iostream>

class Message 
{
public:
    Message(const char* str);
    Message(std::string& str);

    void resize(int length) { message.resize(length); lengthNow = message.size(); }
    void reserve(int length) { message.reserve(length); }
    void copy(const char* str, int length);
    int length() { return message.size(); }
    void clear() { message.clear(); }

    //测试所用
    void show() 
    {
        for (auto x : message) 
        {
            std::cout << x << std::endl;
        }
    }

private:
    std::vector<char> message;
    int lengthNow;
    uint32_t type;      //服务类型 TODO
};

#endif