#pragma once

#include "event.hpp"
#include "order_book.hpp"
#include "thread_safe_queue.hpp"

#include <atomic>
#include <thread>

namespace ome {

class Engine {
public:
    Engine() = default;
    ~Engine() { stop(); }

    void start();
    void stop();

    // Producer API
    void submit(Event ev);

    // For demo / inspection (call only after stop, or add your own locking)
    const OrderBook& book() const { return book_; }

private:
    void run();

    OrderBook book_;
    ThreadSafeQueue<Event> q_;
    std::thread worker_;
    std::atomic<bool> running_{false};
};

} // namespace ome
