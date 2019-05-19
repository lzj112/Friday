#pragma once

#include <fcntl.h>
#include <unistd.h>

class FileDes 
{
public:
    FileDes() : fileFd(-1), isClose(false) {}
    FileDes(int fd) : fileFd(fd), isClose(false) {}
    FileDes(const FileDes&) = delete;
    FileDes& operator=(const FileDes&) = delete;
    ~FileDes() 
    { 
        if (!isClose) 
            ::close(fileFd); 
    }

    void reSet(int fd) 
    {
        close();
        fileFd = fd;
        isClose = false;
    }
    void close()
    {
        if (!isClose) 
        {
            ::close(fileFd);
            isClose = true;
        }
    }

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

