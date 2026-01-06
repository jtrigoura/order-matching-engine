#pragma once

#include <cstdint>

namespace ome {

enum class Side {
    Buy,
    Sell
};

struct Order {
    std::uint64_t id;
    Side side;
    double price;
    std::uint32_t quantity;
};

} // namespace ome
