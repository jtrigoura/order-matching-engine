#pragma once

#include "order.hpp"

#include <map>
#include <deque>
#include <vector>
#include <functional>

namespace ome {

struct Trade {
    Price price;
    std::uint32_t quantity;
    std::uint64_t buy_id;
    std::uint64_t sell_id;
};

class OrderBook {
public:
    void add_order(Order order);
    void match();
    void print() const;

    const std::vector<Trade>& trades() const { return trades_; }
    void clear_trades() { trades_.clear(); }

private:
    using PriceLevel = std::deque<Order>;

    std::map<Price, PriceLevel, std::greater<Price>> bids_;
    std::map<Price, PriceLevel> asks_;

    std::vector<Trade> trades_;

    void remove_empty_front_levels();
};

} // namespace ome