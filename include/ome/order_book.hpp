#pragma once

#include "order.hpp"

#include <map>
#include <deque>
#include <functional> // for std::greater

namespace ome {

struct Trade {
    Price price;
    std::uint32_t quantity;
    std::uint64_t buy_id;
    std::uint64_t sell_id;
};

class OrderBook {
public:
    void add_order(Order order);     // NOTE: by value (we will move it into containers)
    void match();                    // run matching until no crosses
    void print() const;

private:
    using PriceLevel = std::deque<Order>;

    std::map<Price, PriceLevel, std::greater<Price>> bids_;
    std::map<Price, PriceLevel> asks_;

    void remove_empty_front_levels();
};

} // namespace ome
