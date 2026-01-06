#include "ome/order_book.hpp"

#include <iostream>

namespace ome {

void OrderBook::add_order(const Order& order) {
    if (order.side == Side::Buy) {
        bids_[order.price].push_back(order);
    } else {
        asks_[order.price].push_back(order);
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

} // namespace ome
