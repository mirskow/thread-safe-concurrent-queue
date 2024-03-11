#pragma once
#include "SafeConcurrentQueue.h"

class Consumer
{
public:
    Consumer(SafeConcurrentQueue& q) : queue(q) {};

    void execute()
    {
        while (!queue.isStopped() || !queue.isEmpty())
        {
            std::function<void()> foo = this->queue.popTask();
            if (foo)
            {
                foo();
            }
        }
    }

private:
    SafeConcurrentQueue& queue;
};

