#include <gtest/gtest.h>
#include "ome/order_book.hpp"

using namespace ome;

TEST(OrderBookTests, NoCross_NoTrades) {
    OrderBook book;
    book.add_order({1, Side::Buy, 10000, 10});   // bid $100.00
    book.add_order({2, Side::Sell, 10100, 10});  // ask $101.00

    book.match();

    ASSERT_TRUE(book.trades().empty());
}

TEST(OrderBookTests, BasicCross_OneTrade) {
    OrderBook book;
    book.add_order({1, Side::Buy, 10050, 10});   // bid $100.50
    book.add_order({2, Side::Sell, 10000, 7});   // ask $100.00

    book.match();

    ASSERT_EQ(book.trades().size(), 1u);
    const Trade& t = book.trades()[0];
    EXPECT_EQ(t.price, 10000);     // we execute at ask (as implemented)
    EXPECT_EQ(t.quantity, 7u);
    EXPECT_EQ(t.buy_id, 1u);
    EXPECT_EQ(t.sell_id, 2u);
}

TEST(OrderBookTests, FIFO_SamePriceLevel) {
    OrderBook book;
    // Two buys at same price -> order 1 should fill before order 2
    book.add_order({1, Side::Buy, 10050, 5});
    book.add_order({2, Side::Buy, 10050, 5});
    book.add_order({3, Side::Sell, 10050, 7}); // crosses at same price

    book.match();

    // Expected:
    // - First trade: buy_id=1 qty=5
    // - Second trade: buy_id=2 qty=2
    ASSERT_EQ(book.trades().size(), 2u);

    EXPECT_EQ(book.trades()[0].buy_id, 1u);
    EXPECT_EQ(book.trades()[0].sell_id, 3u);
    EXPECT_EQ(book.trades()[0].quantity, 5u);

    EXPECT_EQ(book.trades()[1].buy_id, 2u);
    EXPECT_EQ(book.trades()[1].sell_id, 3u);
    EXPECT_EQ(book.trades()[1].quantity, 2u);
}

TEST(OrderBookTests, PartialFill_LeavesRemainder) {
    OrderBook book;
    book.add_order({1, Side::Buy, 10050, 10});
    book.add_order({2, Side::Sell, 10050, 4}); // partial fill

    book.match();

    ASSERT_EQ(book.trades().size(), 1u);
    EXPECT_EQ(book.trades()[0].quantity, 4u);

    // Now add another sell to consume remaining 6
    book.clear_trades();
    book.add_order({3, Side::Sell, 10050, 6});
    book.match();

    ASSERT_EQ(book.trades().size(), 1u);
    EXPECT_EQ(book.trades()[0].quantity, 6u);
    EXPECT_EQ(book.trades()[0].buy_id, 1u);
    EXPECT_EQ(book.trades()[0].sell_id, 3u);
}

TEST(OrderBookTests, Cancel_RemovesOrderAndPreventsTrade) {
    OrderBook book;

    book.add_order({1, Side::Buy, 10050, 10});
    book.add_order({2, Side::Sell, 10050, 10});

    ASSERT_TRUE(book.cancel_order(1)); // cancel the buy
    book.match();

    ASSERT_TRUE(book.trades().empty());
}

TEST(OrderBookTests, Cancel_NonexistentReturnsFalse) {
    OrderBook book;
    book.add_order({1, Side::Buy, 10050, 10});
    EXPECT_FALSE(book.cancel_order(999));
}

TEST(OrderBookTests, Cancel_FIFOIntegrity_RemovesMiddleOrder) {
    OrderBook book;

    book.add_order({1, Side::Buy, 10050, 5});
    book.add_order({2, Side::Buy, 10050, 5});
    book.add_order({3, Side::Buy, 10050, 5});

    ASSERT_TRUE(book.cancel_order(2)); // remove middle

    book.add_order({4, Side::Sell, 10050, 10});
    book.match();

    // Remaining buys are 1 then 3 (FIFO preserved among remaining)
    ASSERT_EQ(book.trades().size(), 2u);
    EXPECT_EQ(book.trades()[0].buy_id, 1u);
    EXPECT_EQ(book.trades()[0].quantity, 5u);

    EXPECT_EQ(book.trades()[1].buy_id, 3u);
    EXPECT_EQ(book.trades()[1].quantity, 5u);
}
