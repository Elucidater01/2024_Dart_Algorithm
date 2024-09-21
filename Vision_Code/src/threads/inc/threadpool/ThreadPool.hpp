#ifndef THREAD_POOL_HPP_
#define THREAD_POOL_HPP_
#include <iostream>
#include <queue>
#include <mutex>
#include <thread>
#include <functional>
#include <condition_variable>
#include <atomic>
class ThreadPool{
public:
    ThreadPool() = default;
    ~ThreadPool();

    template<typename T>
    void addTask(T &&t);
    void run();
private:
    void launch_worker_once();
    std::vector<std::  thread> workers_;
    std::queue<std::function<void()>> tasks_;
    std::condition_variable condition_;
    std::mutex queue_mutex_;
    std::atomic_bool stop_ = false;
};

inline void ThreadPool::launch_worker_once() {
    this->workers_.emplace_back(
            [this] {
                for ( ; ; ) {
                    std::function<void()> task;
                    {
                        std::unique_lock<std::mutex> lock(this->queue_mutex_);
                        this->condition_.wait(lock, [this]{return this->stop_ || !this->tasks_.empty();});
                        if (this->stop_ && this->tasks_.empty()) {
                            return;
                        }
                        task = std::move(this->tasks_.front());
                        this->tasks_.pop();
                    }
                    task();
                }
            }
    );
}

inline void ThreadPool::run() {
    {
        std::unique_lock<std::mutex> lock(this->queue_mutex_);
        this->stop_ = true;
    }

    this->condition_.notify_all();
    for (std::thread &worker : workers_) {
        worker.join();
    }
}

ThreadPool::~ThreadPool() = default;

template<typename T>
void ThreadPool::addTask(T &&t) {
    this->launch_worker_once();

    std::unique_lock<std::mutex> lock(this->queue_mutex_);

    if (this->stop_) {
        throw std::runtime_error("thread pool already stopped");
    }

    this->tasks_.emplace(t);
    this->condition_.notify_one();
}

#endif