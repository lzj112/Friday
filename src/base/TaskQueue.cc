
#include "../base/TaskQueue.h"

template<class T>
void TaskQueue<T>::getNextTask(T& job) 
{
    if (!empty() && nextTask < size())
    {
        job = taskQueue[nextTask];
        ++nextTask;
    }

}

template<class T>
void TaskQueue<T>::add(const T&& job) 
{
    taskQueue.push_back(job);
}