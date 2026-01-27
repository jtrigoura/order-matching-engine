#include "ome/order_book.hpp"

#include <iostream>
#include <utility>

namespace ome {

void OrderBook::add_order(Order order) {
    if (order.quantity == 0) return;

    if (order.side == Side::Buy) {
        bids_[order.price].push_back(std::move(order));
    } else {
        asks_[order.price].push_back(std::move(order));
    }
}

void OrderBook::remove_empty_front_levels() {
    // remove empty best bid levels
    while (!bids_.empty() && bids_.begin()->second.empty()) {
        bids_.erase(bids_.begin());
    }
    // remove empty best ask levels
    while (!asks_.empty() && asks_.begin()->second.empty()) {
        asks_.erase(asks_.begin());
    }
}

void OrderBook::match() {
    remove_empty_front_levels();
    while (has_cross()) {
        execute_one_trade();
        remove_empty_front_levels();
    }
}

void OrderBook::print() const {
    std::cout << "BIDS:\n";
    for (const auto& [price, queue] : bids_) {
        std::cout << "  Price " << price << " -> ";
        for (const auto& order : queue) {
            std::cout << order.id << "(" << order.quantity << ") ";
        }
        std::cout << "\n";
    }

    std::cout << "ASKS:\n";
    for (const auto& [price, queue] : asks_) {
        std::cout << "  Price " << price << " -> ";
        for (const auto& order : queue) {
            std::cout << order.id << "(" << order.quantity << ") ";
        }
        std::cout << "\n";
    }
}

bool OrderBook::has_cross() const {
    if (bids_.empty() || asks_.empty()) return false;
    const Price best_bid = bids_.begin()->first; // highest
    const Price best_ask = asks_.begin()->first; // lowest
    return best_bid >= best_ask;
}

bool OrderBook::execute_one_trade() {
    // Precondition: has_cross() is true and empty levels are cleaned
    auto best_bid_it = bids_.begin();
    auto best_ask_it = asks_.begin();

    auto& bid_q = best_bid_it->second;
    auto& ask_q = best_ask_it->second;

    Order& buy  = bid_q.front();
    Order& sell = ask_q.front();

    const std::uint32_t traded_qty =
        (buy.quantity < sell.quantity) ? buy.quantity : sell.quantity;

    // Execution price policy: trade at resting ask (deterministic)
    const Price trade_price = best_ask_it->first;

    trades_.push_back(Trade{trade_price, traded_qty, buy.id, sell.id});

    buy.quantity  -= traded_qty;
    sell.quantity -= traded_qty;

    if (buy.quantity == 0)  bid_q.pop_front();
    if (sell.quantity == 0) ask_q.pop_front();

    return true;
}
namespace {

// Erase first order with matching id from a deque.
// Returns true if removed.
bool erase_from_level(std::deque<ome::Order>& q, std::uint64_t id) {
    for (auto it = q.begin(); it != q.end(); ++it) {
        if (it->id == id) {
            q.erase(it);
            return true;
        }
    }
    return false;
}

} // namespace

bool OrderBook::cancel_in_side(std::map<Price, PriceLevel, std::greater<Price>>& side, std::uint64_t id) {
    for (auto it = side.begin(); it != side.end(); ++it) {
        auto& level = it->second;
        if (erase_from_level(level, id)) {
            // cleanup empty level
            if (level.empty()) side.erase(it);
            return true;
        }
    }
    return false;
}

bool OrderBook::cancel_in_side(std::map<Price, PriceLevel>& side, std::uint64_t id) {
    for (auto it = side.begin(); it != side.end(); ++it) {
        auto& level = it->second;
        if (erase_from_level(level, id)) {
            if (level.empty()) side.erase(it);
            return true;
        }
    }
    return false;
}

bool OrderBook::cancel_order(std::uint64_t order_id) {
    // Try bids first, then asks
    if (cancel_in_side(bids_, order_id)) return true;
    if (cancel_in_side(asks_, order_id)) return true;
    return false;
}

} // namespace ome
