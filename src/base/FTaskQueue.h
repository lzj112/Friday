#pragma once

#include <list>
#include <mutex>
#include <thread>
#include <condition_variable>


template <class T>
class FTaskQueue 
{
public:
    FTaskQueue(int maxSize = 256);
    ~FTaskQueue();

    bool isEmpty() const;
    bool isFull() const;
    int size();
    void putTask(const T& task);
    void takeTask(T& task);
    void takeTask(std::list<T>& list);
    void stop();

private:
    void stopQueue();

private:
    std::list<T> Fqueue;
    std::mutex Fmutex;
    std::condition_variable notEmpty_;
    std::condition_variable notFull_;
    int maxQueueSize;
    bool isStop;
    std::once_flag queueFlag;
};

