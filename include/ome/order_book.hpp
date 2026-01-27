#pragma once

#include "order.hpp"

#include <map>
#include <deque>
#include <vector>
#include <functional>
#include <cstdint>

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
    bool cancel_order(std::uint64_t order_id);

    void match();
    void print() const;

    const std::vector<Trade>& trades() const { return trades_; }
    void clear_trades() { trades_.clear(); }

private:
    using PriceLevel = std::deque<Order>;

    // bids: highest price first
    std::map<Price, PriceLevel, std::greater<Price>> bids_;
    // asks: lowest price first
    std::map<Price, PriceLevel> asks_;

    // trade log for testing/inspection
    std::vector<Trade> trades_;

    void remove_empty_front_levels();

    bool has_cross() const;
    bool execute_one_trade();

    // cancellation helpers (overloads because map comparator differs)
    bool cancel_in_side(std::map<Price, PriceLevel, std::greater<Price>>& side, std::uint64_t id);
    bool cancel_in_side(std::map<Price, PriceLevel>& side, std::uint64_t id);
};

} // namespace ome
