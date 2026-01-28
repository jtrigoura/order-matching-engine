#pragma once

#include <condition_variable>
#include <mutex>
#include <queue>
#include <utility>

namespace ome {

template <typename T>
class ThreadSafeQueue {
public:
    void push(T value) {
        {
            std::lock_guard<std::mutex> lock(m_);
            q_.push(std::move(value));
        }
        cv_.notify_one();
    }

    // Blocks until an item is available or stop() is called.
    bool wait_pop(T& out) {
        std::unique_lock<std::mutex> lock(m_);
        cv_.wait(lock, [&]{ return stopped_ || !q_.empty(); });

        if (stopped_ && q_.empty()) return false;

        out = std::move(q_.front());
        q_.pop();
        return true;
    }

    void stop() {
        {
            std::lock_guard<std::mutex> lock(m_);
            stopped_ = true;
        }
        cv_.notify_all();
    }

private:
    std::mutex m_;
    std::condition_variable cv_;
    std::queue<T> q_;
    bool stopped_{false};
};

} // namespace ome
