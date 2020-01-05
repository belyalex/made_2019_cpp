//
// Created by alex on 05.01.2020.
//

#ifndef CPP_TASK09_THREADPOOL_H
#define CPP_TASK09_THREADPOOL_H

#include <vector>
#include <queue>
#include <future>
#include <functional>

class ThreadPool
{
public:
    explicit ThreadPool(size_t poolSize) : working(true) {
        for(size_t i = 0; i < poolSize; i++) {
            threads.push_back(std::thread(std::bind(&ThreadPool::worker, this)));
        }
    }

    ~ThreadPool() {
        working=false;
        condition.notify_all();
        for (auto &thread : threads) {
            thread.join();
        }
    }

    // pass arguments by value
    template <class Func, class... Args>
    auto exec(Func func, Args... args) -> std::future<decltype(func(args...))> {
        using PackagedTask = std::packaged_task<decltype(func(args...))()>;
        PackagedTask* task = new PackagedTask(std::bind(func, args...));
        auto future = task->get_future();
        {
            std::unique_lock<std::mutex> lock(mutex);
            tasks.push([task]() {
                (*task)();
                delete task;
                return;
            });
        }
        condition.notify_one();
        return future;
    }
private:
    void worker() {
        while (working) {
            std::unique_lock<std::mutex> lock(mutex);
            if (!tasks.empty()) {
                auto task=tasks.front();
                tasks.pop();
                lock.unlock();
                task();
            }
            else {
                condition.wait(lock);
            }
        }
    }

    std::vector<std::thread> threads;
    std::queue<std::function<void()>> tasks;

    std::mutex mutex;
    std::condition_variable condition;

    std::atomic<bool> working;
};

#endif //CPP_TASK09_THREADPOOL_H
