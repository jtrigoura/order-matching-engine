#include <gtest/gtest.h>
#include "ome/engine.hpp"
#include <thread>

using namespace ome;

TEST(EngineTests, ProcessesEventsAndMatches) {
    Engine engine;
    engine.start();

    std::thread p1([&]{
        engine.submit(NewOrder{Order{1, Side::Buy, 10050, 10}});
    });

    std::thread p2([&]{
        engine.submit(NewOrder{Order{2, Side::Sell, 10050, 7}});
    });

    p1.join();
    p2.join();

    engine.stop();

    ASSERT_EQ(engine.book().trades().size(), 1u);
    EXPECT_EQ(engine.book().trades()[0].quantity, 7u);
}

TEST(EngineTests, CancelEventPreventsTrade) {
    Engine engine;
    engine.start();

    engine.submit(NewOrder{Order{1, Side::Buy, 10050, 10}});
    engine.submit(CancelOrder{1});
    engine.submit(NewOrder{Order{2, Side::Sell, 10050, 10}});

    engine.stop();

    ASSERT_TRUE(engine.book().trades().empty());
}
