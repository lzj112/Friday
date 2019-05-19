
#include "FTaskQueue.h" 

template<class T>
FTaskQueue<T>::FTaskQueue(int maxSize) 
    : maxQueueSize(maxSize),
      isStop(false)
{}

template<class T>
FTaskQueue<T>::~FTaskQueue() 
{
    try 
    {
        std::call_once(queueFlag, 
                   [this]{
                       stopQueue();
                   });
    }
    catch (...) 
    {}
}


template<class T>
bool FTaskQueue<T>::isEmpty() const
{
    std::lock_guard<std::mutex> locker(Fmutex);
    return Fqueue.empty();
}

template<class T> 
bool FTaskQueue<T>::isFull() const
{
    std::lock_guard<std::mutex> locker(Fmutex);
    return Fqueue.size() >= maxQueueSize;
}

template<class T>
int FTaskQueue<T>::size() 
{
    std::lock_guard<std::mutex>  locker(Fmutex);
    return Fqueue.size();
}

template<class T> 
void FTaskQueue<T>::putTask(const T& task) 
{
    std::unique_lock<std::mutex> locker(Fmutex);
    //在终止时或者队列未满时返回
    notFull_.wait(locker,
                  [this]{
                    return isStop || !isFull();    
                  });
    if (isStop)
        return ;
    Fqueue.emplace_back(std::forward<T&&> (task));
    //唤醒因为任务队列为空而阻塞的线程
    notEmpty_.notify_one();
}

template<class T>
void FTaskQueue<T>::takeTask(T& task) 
{
    std::unique_lock<std::mutex> locker(Fmutex);
    //终止时或者任务队列不为空就返回
    notEmpty_.wait(locker, 
                   [this]{
                       return isStop || !isEmpty();
                   });
    if (isStop) 
        return ;
    
    task = Fqueue.front();
    Fqueue.pop_front();
    //唤醒因为队列满而导致添加任务阻塞的线程
    notFull_.notify_one();  
}

template<class T> 
void FTaskQueue<T>::takeTask(std::list<T>& list) 
{
    std::unique_lock<std::mutex> locker(Fmutex);
    //终止时或者任务队列不为空就返回
    notEmpty_.wait(locker, 
                   [this]{
                       return isStop || !isEmpty();
                   });
    if (isStop) 
        return ;
    list = std::move(Fqueue);
    //唤醒因为队列满而导致添加任务阻塞的线程
    notFull_.notify_one();  
}

template<class T> 
void FTaskQueue<T>::stop() 
{
    std::call_once(queueFlag, 
                   [this]{
                       stopQueue();
                   });
}

template<class T> 
void FTaskQueue<T>::stopQueue() 
{
    {
        std::lock_guard<std::mutex> locker(Fmutex);
        isStop = true;
    }

    notFull_.notify_all();
    notEmpty_.notify_all();
}