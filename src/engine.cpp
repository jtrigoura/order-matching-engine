#include "ome/engine.hpp"

#include <utility>
#include <variant>

namespace ome {

void Engine::start() {
    if (running_.exchange(true)) return;
    worker_ = std::thread(&Engine::run, this);
}

void Engine::stop() {
    if (!running_.exchange(false)) return;
    q_.stop();
    if (worker_.joinable()) worker_.join();
}

void Engine::submit(Event ev) {
    q_.push(std::move(ev));
}

void Engine::run() {
    Event ev;
    while (q_.wait_pop(ev)) {
        std::visit([&](auto&& msg) {
            using M = std::decay_t<decltype(msg)>;
            if constexpr (std::is_same_v<M, NewOrder>) {
                book_.add_order(std::move(msg.order));
                book_.match();
            } else if constexpr (std::is_same_v<M, CancelOrder>) {
                book_.cancel_order(msg.order_id);
                book_.match();
            }
        }, ev);
    }
}

} // namespace ome
