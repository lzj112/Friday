
#include <errno.h>

#include <string>
#include <utility>
#include <iostream>
#include <exception>

#include "TimerWheel.h"

TimerWheel::TimerWheel(EpollEventLoop* loop) 
    : loop_(loop),
      currentSlot(0),
      timerfd_(timerfd_create(CLOCK_REALTIME, TFD_NONBLOCK)),
      defaultTimerCallBack_(std::bind(&TimerWheel::defaultTimerCallBack, 
                                      this))
{
    wheel.resize(N);
    start();
}

TimerWheel::~TimerWheel() 
{
    
}

void TimerWheel::start() 
{
    MyEvent timeTmp(loop_, timerfd_.fd());
    timeTmp.setMessMana(std::bind(&TimerWheel::readTimerfd,
                                  this,
                                  std::placeholders::_1,
                                  std::placeholders::_2));
    // timeTmp.setReadCallBack(std::bind(&TimerWheel::readTimerfd, this));

    loop_->regReadable(timeTmp);

    itimerspec new_value;
    new_value.it_value.tv_sec = SI; //设置第一次到期时间
    new_value.it_value.tv_nsec = 0;
    new_value.it_interval.tv_sec = SI;  //设置tick时间
    new_value.it_interval.tv_nsec = 0;
    //启动timerfd定时器
    int ret = timerfd_settime(timerfd_.fd(), 0, &new_value, nullptr);
    assert(ret != -1);
}

void TimerWheel::readTimerfd(const MyEvent*, const Message&) 
{
    printf("定时器 timerfd 到期\n");
    ssize_t s;
    uint64_t exp;
    s = ::read(timerfd_.fd(), &exp, sizeof(uint64_t));
    assert(s == sizeof(uint64_t));

    tick();
}

uint32_t TimerWheel::addTimer(int firstTime, 
                              int interval, 
                              timerCallBack cb = nullptr,
                              int fd = -1) 
{
    if (cb == nullptr) 
    {
        cb = defaultTimerCallBack_;
    }
    if (firstTime < 0 || interval < 0) return -1;
    
    //多少次SI后被触发
    int timeout = (firstTime < SI) ? 1 : (firstTime / SI);
    // //转动多少圈后被触发
    // int rotation = timeout / N;
    //应该插入哪个槽
    int ts = (currentSlot + (timeout % N)) % N;

    Timer timerTmp(firstTime, interval, cb, fd);
    uint32_t timerID = timerTmp.id();
    //记录timerid和定时器指针的对应关系
    location.emplace(std::make_pair(timerID, std::move(timerTmp)));
    //插入该槽
    wheel[ts].emplace(std::move(timerTmp));

    return timerID;
}

void TimerWheel::cancleTimer(uint32_t timerID) 
{
    auto it = location.find(timerID);
    if (it != location.end()) 
    {
        for (int i = 0; i < N; i++) 
        {
            auto iter = wheel[i].find(it->second);
            wheel[i].erase(iter);
        }
    }
    return ;
}


void TimerWheel::tick() 
{
    if (!wheel[currentSlot].empty()) 
    {
        auto it = wheel[currentSlot].begin();
        auto end = wheel[currentSlot].end();
        timespec now;

        for (; it != end;) 
        {
            clock_gettime(CLOCK_REALTIME, &now);
            int timeNow = static_cast<int> (now.tv_sec);
            int timeStamp = it->expiration();

            if (timeStamp <= timeNow) 
            {
                it->tick();
                if (it->isRepeat()) 
                {
                    int expire = it->interval();
                    int interval = it->interval();
                    int fd = it->fd();
                    timerCallBack cb = it->timerFunc();

                    it = wheel[currentSlot].erase(it);

                    addTimer(expire,
                             interval,
                             cb,
                             fd);
                }
                else 
                {
                    it = wheel[currentSlot].erase(it);
                }
            }
            else 
            {
                it++;
            }
        }
    }

    currentSlot = ++currentSlot % N;    
}

void TimerWheel::defaultTimerCallBack() 
{
    std::cout << "performing defaultTimerCallBack " << std::endl;
}
