#pragma once

#include <iostream>
#include <thread>
#include <mutex>
#include <queue>
#include <functional>
#include <condition_variable>

class SafeConcurrentQueue
{
private:
    std::queue<std::function<void()>> ConcurrentQueue;
    std::mutex m;
    std::atomic<bool> stop_f;
    std::condition_variable cv;

public:

    void pushTask(std::function<void()> task)
    {
        std::unique_lock<std::mutex> ul(m);
        ConcurrentQueue.push(task);
        cv.notify_all();
    }

    std::function<void()> popTask()
    {
        std::unique_lock<std::mutex> ul(m);
        cv.wait(ul, [this]() { return !ConcurrentQueue.empty() || stop_f.load(); });

        if (!ConcurrentQueue.empty()) {
            std::function<void()> foo = ConcurrentQueue.front();
            ConcurrentQueue.pop();
            return foo;
        }

        return {};
    }

    void run()
    {
        this->stop_f.store(false);
    }

    void stop()
    {
        this->stop_f.store(true);
        cv.notify_all();
    }

    bool isStopped() const
    {
        return stop_f.load();
    }

    bool isEmpty()
    {
        return ConcurrentQueue.empty();
    }
};
