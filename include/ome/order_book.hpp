#pragma once

#include "order.hpp"

#include <map>
#include <deque>

namespace ome {

class OrderBook {
public:
    void add_order(const Order& order);
    void print() const;

private:
    using PriceLevel = std::deque<Order>;

    std::map<double, PriceLevel> bids_;
    std::map<double, PriceLevel> asks_;
};

} // namespace ome
