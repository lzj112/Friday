#ifndef FILEDES_H_
#define FILEDES_H_

#include <fcntl.h>
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

    void setNonBlocking() 
    {
        int old_option = fcntl(fileFd, F_GETFL, 0);
        int new_option = old_option | O_NONBLOCK;
        fcntl(fileFd, F_SETFL, new_option);
    }

private:
    int fileFd;
    bool isClose;
};

#endif