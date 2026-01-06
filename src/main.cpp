#include "ome/order_book.hpp"

using namespace ome;

int main() {
    OrderBook book;

    book.add_order({1, Side::Buy, 100.5, 10});
    book.add_order({2, Side::Buy, 100.5, 5});
    book.add_order({3, Side::Sell, 101.0, 7});
    book.add_order({4, Side::Sell, 101.0, 3});

    book.print();
}
