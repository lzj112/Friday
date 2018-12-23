#include <cstring>

#include <iostream>
#include <thread>
#include <vector>
#include <functional>


// #include "Message.h"
#include "../base/TaskQueue.h"

using namespace std;

class A 
{
public:
    A() {}
    void put(int val)
    {
        // job->add(val);
        job.add(val);
    }
    void show() 
    {
        int val;
        // job->getNextTask(val);
        job.getNextTask(val);
        cout << val << endl;
    }
private:
    TaskQueue<int> job;
    // std::unique_ptr<TaskQueue<int> > job;
};

int main() 
{
    A t;
    t.put(1024);
    t.show();
}