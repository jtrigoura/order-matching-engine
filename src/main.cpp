#include "ome/order_book.hpp"

using namespace ome;

int main() {
    OrderBook book;

    // Prices in cents
    book.add_order({1, Side::Buy, 10050, 10}); // $100.50 bid qty 10
    book.add_order({2, Side::Buy, 10050, 5});  // same price, FIFO behind id=1
    book.add_order({3, Side::Sell, 10000, 7}); // $100.00 ask qty 7  (crosses)
    book.add_order({4, Side::Sell, 10050, 20}); // $100.50 ask qty 20 (crosses)

    book.match();
    book.print();
}
