#pragma once

#include "order.hpp"
#include <cstdint>
#include <variant>

namespace ome {

struct NewOrder {
    Order order;
};

struct CancelOrder {
    std::uint64_t order_id;
};

using Event = std::variant<NewOrder, CancelOrder>;

} // namespace ome
