#ifndef TASKQUEUE_H_
#define TASKQUEUE_H_

#include <vector>

#include "../net/EpollBase.h"

template<class T>
class TaskQueue 
{
public:
    TaskQueue() : nextTask(0), isEmpty(true) {}
    ~TaskQueue() {}
    
    int size() { return taskQueue.size(); }
    bool empty() const
    { return taskQueue.size() == 0 ? true : false; }
    int count() const 
    { return taskQueue.size(); }
    void cleanJob() 
    { taskQueue.clear(); isEmpty = true; nextTask = 0; }

    void add(T& job) 
    { taskQueue.push_back(job); }

    void getNextTask(T& job)
    {
        if (!empty() && nextTask < size())
        {
            job = taskQueue[nextTask];
            ++nextTask;
        }

    }

private:

    std::vector<T> taskQueue;
    // Mutex locker;

    int nextTask;
    bool isEmpty;
};

// template class TaskQueue<int>;
// template class TaskQueue<IOcallBack&>;

#endif