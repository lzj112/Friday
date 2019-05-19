#pragma once

#include <list>
#include <mutex>
#include <thread>
#include <condition_variable>


template <class T>
class FTaskQueue 
{
public:
    FTaskQueue();
    ~FTaskQueue();

    bool isEmpty();
    bool isFUll();
    int size();
    void putTask(const T& x);
    void putTask(T&& x);
    void takeTask(T& x);
    void takeTask(std::list<T>& list);
    void stop();

private:
    bool notFull() const;
    bool notEmpty() const;

    void add(T&& x);
    void add(const T& x);

private:
    std::list<T> fFaskQueue;
    std::mutex fMutex;
    std::condition_variable notEmpty_;
    std::condition_variable notFull_;
    int maxQueueSize;
    bool needStop;
};

