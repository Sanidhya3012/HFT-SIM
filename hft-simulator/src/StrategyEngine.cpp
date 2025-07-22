#include "StrategyEngine.h"
#include <iostream>
#include <random>

StrategyEngine::StrategyEngine(OrderBook& order_book, double spread, int interval_ms)
    : order_book_(order_book), spread_(spread), interval_ms_(interval_ms), running_(false), next_order_id_(10000) {}

StrategyEngine::~StrategyEngine() {
    stop();
}

void StrategyEngine::start() {
    if (running_) return;
    running_ = true;
    worker_ = std::thread(&StrategyEngine::run, this);
}

void StrategyEngine::stop() {
    if (!running_) return;
    running_ = false;
    if (worker_.joinable()) {
        worker_.join();
    }
}

bool StrategyEngine::isRunning() const {
    return running_;
}

// Main loop: periodically submit buy/sell orders at the spread
void StrategyEngine::run() {
    std::default_random_engine rng(std::random_device{}());
    std::uniform_real_distribution<double> price_dist(99.0, 101.0); // Simulated mid-price
    std::uniform_int_distribution<int> qty_dist(1, 10);

    while (running_) {
        double mid_price = price_dist(rng);
        double buy_price = mid_price - spread_ / 2.0;
        double sell_price = mid_price + spread_ / 2.0;
        int buy_qty = qty_dist(rng);
        int sell_qty = qty_dist(rng);
        std::uint64_t ts = static_cast<std::uint64_t>(std::chrono::system_clock::now().time_since_epoch().count());

        // Submit buy order
        Order buy_order(next_order_id_++, Order::Side::BUY, buy_price, buy_qty, ts);
        order_book_.addOrder(buy_order);
        std::cout << "[Strategy] Submitted BUY order: Price=" << buy_price << ", Qty=" << buy_qty << std::endl;

        // Submit sell order
        Order sell_order(next_order_id_++, Order::Side::SELL, sell_price, sell_qty, ts);
        order_book_.addOrder(sell_order);
        std::cout << "[Strategy] Submitted SELL order: Price=" << sell_price << ", Qty=" << sell_qty << std::endl;

        // Wait for the next interval
        std::this_thread::sleep_for(std::chrono::milliseconds(interval_ms_));
    }
} 