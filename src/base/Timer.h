#ifndef TIMER_H_
#define TIMER_H_

#include <unistd.h>
#include <assert.h>
#include <sys/timerfd.h>

#include <iostream>

using namespace std;

enum TIMERTYPE 
{
    RELATIVE = CLOCK_REALTIME,  //相对时间
    ABSOLUTE = CLOCK_MONOTONIC  //绝对时间
};

//默认是相对时间
class Timer 
{
public:
    Timer(int type = RELATIVE);
    ~Timer() { ::close(timerfd_); }
    void setUpTimer(int firstTime, int interval);
    void shutdown();
    int fd();

private:

    void startTimer(const itimerspec&);
    int type_;
    int timerfd_;   //定时时间  time_t就是long int
};

#endif