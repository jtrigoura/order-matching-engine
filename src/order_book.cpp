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

    while (!bids_.empty() && !asks_.empty()) {
        auto best_bid_it = bids_.begin(); // highest price because std::greater
        auto best_ask_it = asks_.begin(); // lowest price

        const Price best_bid = best_bid_it->first;
        const Price best_ask = best_ask_it->first;

        if (best_bid < best_ask) {
            // no cross -> done
            break;
        }

        auto& bid_q = best_bid_it->second;
        auto& ask_q = best_ask_it->second;

        // FIFO: front of each queue
        Order& buy  = bid_q.front();
        Order& sell = ask_q.front();

        const std::uint32_t traded_qty =
            (buy.quantity < sell.quantity) ? buy.quantity : sell.quantity;

        // Simple execution price rule:
        // trade at resting ask (typical for a buy taking liquidity) or resting price.
        // Here we'll use best_ask for determinism.
        const Price trade_price = best_ask;

        std::cout << "TRADE price=" << trade_price
                  << " qty=" << traded_qty
                  << " buy_id=" << buy.id
                  << " sell_id=" << sell.id << "\n";

        buy.quantity  -= traded_qty;
        sell.quantity -= traded_qty;

        if (buy.quantity == 0) {
            bid_q.pop_front();
        }
        if (sell.quantity == 0) {
            ask_q.pop_front();
        }

        // clean up empty price levels
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

} // namespace ome
