#include "StrategyEngine.h"
#include <iostream>
#include <random>
// #include <thread> // Threading disabled for compatibility

StrategyEngine::StrategyEngine(OrderBook& order_book, double spread, int interval_ms)
    : order_book_(order_book), spread_(spread), interval_ms_(interval_ms), running_(false), next_order_id_(10000) {}

StrategyEngine::~StrategyEngine() {
    stop();
}

// Threading disabled: start is a no-op
void StrategyEngine::start() {
    // No background thread
    std::cout << "[StrategyEngine] Threading is disabled. No orders will be submitted automatically.\n";
}

// Threading disabled: stop is a no-op
void StrategyEngine::stop() {
    // No background thread
}

// Threading disabled: always returns false
bool StrategyEngine::isRunning() const {
    return false;
}

// Main loop for submitting orders (not used)
void StrategyEngine::run() {
    // Threading is disabled; this function is not used.
} 