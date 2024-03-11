#include <chrono>
#include <iostream>
#include <thread>
#include <mutex>
#include <queue>
#include <functional>
#include <vector>
#include "SafeConcurrentQueue.h"
#include "Producer.h"
#include "Consumer.h"

int main()
{
    std::setlocale(LC_ALL, "");

    SafeConcurrentQueue q;
    q.run();

    auto start = std::chrono::high_resolution_clock::now();

    std::vector<std::thread> producers;
    for (int i = 0; i < 2; i++) 
    {
        producers.emplace_back([&q]()
        {
            Producer p(q);
            p.producing();
        });
    }

    std::vector<std::thread> consumers;
    for (int i = 0; i < 3; i++)
    {
        consumers.emplace_back([&q]()
        {
            Consumer c(q);
            c.execute();
        });
    }

    std::this_thread::sleep_for(std::chrono::seconds(20));
    q.stop();

    for (auto& producer : producers) { producer.join();  }
    for (auto& consumer : consumers) { consumer.join(); }
    
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> duration = end - start;

    std::cout << duration.count() << " - program running time" << std::endl;
    
}