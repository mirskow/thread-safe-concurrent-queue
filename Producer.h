#pragma once
#include "SafeConcurrentQueue.h"

static void test_void()
{
    std::cout << std::this_thread::get_id() << " начал решать задачу." << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(4000));
}

class Producer
{
public:
    Producer(SafeConcurrentQueue& q) : queue(q) {};

    void producing()
    {
        while (!queue.isStopped())
        {
            this->queue.pushTask(test_void);
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
        }
    }

private:
    SafeConcurrentQueue& queue;
};