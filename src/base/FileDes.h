#ifndef FILEDES_H_
#define FILEDES_H_

#include <unistd.h>

class FileDes 
{
public:
    FileDes() : fileFd(-1), isClose(false) {}
    FileDes(int fd) : fileFd(fd), isClose(false) {}
    ~FileDes() 
    { 
        if (!isClose) 
        close(fileFd); 
    }

    void setFd(int fd) { fileFd = fd; isClose = false; }
    void Close() { isClose = true; close(fileFd); }

    int fd() { return fileFd; }

private:
    int fileFd;
    bool isClose;
};

#endif