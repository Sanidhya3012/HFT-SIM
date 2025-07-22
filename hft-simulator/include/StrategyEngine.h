#ifndef STRATEGYENGINE_H
#define STRATEGYENGINE_H

#include "OrderBook.h"
#include <atomic>
#include <chrono>

// Simulates a basic market-making strategy (threading temporarily disabled for compatibility)
class StrategyEngine {
public:
    // spread: price difference between buy and sell
    // interval_ms: time between order submissions (milliseconds)
    StrategyEngine(OrderBook& order_book, double spread, int interval_ms);
    ~StrategyEngine();

    // Start the strategy (no-op)
    void start();
    // Stop the strategy (no-op)
    void stop();
    // Check if running (always false)
    bool isRunning() const;

private:
    // Main loop for submitting orders (not used)
    void run();

    OrderBook& order_book_;
    double spread_;
    int interval_ms_;
    std::atomic<bool> running_;
    int next_order_id_;
};

#endif // STRATEGYENGINE_H 