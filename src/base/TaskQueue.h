#ifndef TASKQUEUE_H_
#define TASKQUEUE_H_

#include <vector>

template<class T>
class TaskQueue 
{
public:
    TaskQueue(T) : nextTask(0), isEmpty(true) {}
    ~TaskQueue();
    
    int size() { return taskQueue.size(); }
    bool empty() const
    { return taskQueue.size() == 0 ? true : false; }
    int count() const 
    { return taskQueue.size(); }
    void cleanJob() 
    { taskQueue.clear(); isEmpty = true; nextTask = 0; }

    void put(const T& job) { std::move<T> (job); }
    void put(const T&& job) { std::forward<T> (job); }

    void getNextTask(T& job);

private:
    void add(const T&& job);

    std::vector<T> taskQueue;
    Mutex locker;

    int nextTask;
    bool isEmpty;
};

#endif