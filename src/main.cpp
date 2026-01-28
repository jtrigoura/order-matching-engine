#include "ome/engine.hpp"
#include <thread>

using namespace ome;

int main() {
    Engine engine;
    engine.start();

    // Producer threads
    std::thread p1([&]{
        engine.submit(NewOrder{Order{1, Side::Buy, 10050, 10}});
        engine.submit(NewOrder{Order{2, Side::Buy, 10050, 5}});
    });

    std::thread p2([&]{
        engine.submit(NewOrder{Order{3, Side::Sell, 10000, 7}});
        engine.submit(CancelOrder{2}); // cancel order 2
        engine.submit(NewOrder{Order{4, Side::Sell, 10050, 20}});
    });

    p1.join();
    p2.join();

    engine.stop();

    // After stop, safe to inspect
    engine.book().print();
}
