#pragma once

#include <cstdint>

namespace ome {

enum class Side { Buy, Sell };

using Price = std::int64_t; // ticks (e.g. cents)

struct Order {
    std::uint64_t id;
    Side side;
    Price price;
    std::uint32_t quantity;
};

} // namespace ome
