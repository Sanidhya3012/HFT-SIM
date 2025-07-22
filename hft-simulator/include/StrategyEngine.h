#ifndef STRATEGYENGINE_H
#define STRATEGYENGINE_H

#include "OrderBook.h"
#include <thread>
#include <atomic>
#include <chrono>

// Simulates a basic market-making strategy by submitting buy/sell orders at a configurable spread and interval
class StrategyEngine {
public:
    // spread: price difference between buy and sell
    // interval_ms: time between order submissions (milliseconds)
    StrategyEngine(OrderBook& order_book, double spread, int interval_ms);
    ~StrategyEngine();

    // Start the strategy in a background thread
    void start();
    // Stop the strategy
    void stop();
    // Check if running
    bool isRunning() const;

private:
    // Main loop for submitting orders
    void run();

    OrderBook& order_book_;
    double spread_;
    int interval_ms_;
    std::atomic<bool> running_;
    std::thread worker_;
    int next_order_id_;
};

#endif // STRATEGYENGINE_H 