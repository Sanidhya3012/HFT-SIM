#include "StrategyEngine.h"
#include <iostream>
#include <random>
// #include <thread> // Threading disabled for compatibility
#include <algorithm>
#include <numeric>

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

// Market Making: quote both bid and ask around mid-price
void MarketMakingStrategy::step() {
    // Estimate mid-price from best buy/sell
    double best_buy = 0.0, best_sell = 0.0;
    if (!order_book_.getBuyOrders().empty())
        best_buy = order_book_.getBuyOrders().front().getPrice();
    if (!order_book_.getSellOrders().empty())
        best_sell = order_book_.getSellOrders().front().getPrice();
    double mid = (best_buy > 0.0 && best_sell > 0.0) ? (best_buy + best_sell) / 2.0 : 100.0;
    double bid = mid - spread_ / 2.0;
    double ask = mid + spread_ / 2.0;
    std::uint64_t ts = static_cast<std::uint64_t>(std::chrono::system_clock::now().time_since_epoch().count());
    order_book_.addOrder(Order(order_id_++, Order::Side::BUY, bid, qty_, ts, Order::OrderType::LIMIT));
    order_book_.addOrder(Order(order_id_++, Order::Side::SELL, ask, qty_, ts, Order::OrderType::LIMIT));
}

// Momentum: go with short-term price trend
void MomentumStrategy::step() {
    // Use last trade price as signal (or best price if no trades)
    double price = 0.0;
    if (!order_book_.getSellOrders().empty())
        price = order_book_.getSellOrders().front().getPrice();
    else if (!order_book_.getBuyOrders().empty())
        price = order_book_.getBuyOrders().front().getPrice();
    else
        price = 100.0;
    if (last_price_ == 0.0) {
        last_price_ = price;
        return;
    }
    std::uint64_t ts = static_cast<std::uint64_t>(std::chrono::system_clock::now().time_since_epoch().count());
    if (price > last_price_) {
        // Uptrend: go long
        order_book_.addOrder(Order(order_id_++, Order::Side::BUY, price + 0.01, qty_, ts, Order::OrderType::LIMIT));
    } else if (price < last_price_) {
        // Downtrend: go short
        order_book_.addOrder(Order(order_id_++, Order::Side::SELL, price - 0.01, qty_, ts, Order::OrderType::LIMIT));
    }
    last_price_ = price;
}

// Mean Reversion: bet on return to moving average
void MeanReversionStrategy::step() {
    // Use best price as current price
    double price = 0.0;
    if (!order_book_.getSellOrders().empty())
        price = order_book_.getSellOrders().front().getPrice();
    else if (!order_book_.getBuyOrders().empty())
        price = order_book_.getBuyOrders().front().getPrice();
    else
        price = 100.0;
    price_history_.push_back(price);
    if ((int)price_history_.size() < window_) return;
    if ((int)price_history_.size() > window_) price_history_.erase(price_history_.begin());
    double mean = std::accumulate(price_history_.begin(), price_history_.end(), 0.0) / window_;
    std::uint64_t ts = static_cast<std::uint64_t>(std::chrono::system_clock::now().time_since_epoch().count());
    if (price < mean - 0.05) {
        // Price below mean: buy
        order_book_.addOrder(Order(order_id_++, Order::Side::BUY, price + 0.01, qty_, ts, Order::OrderType::LIMIT));
    } else if (price > mean + 0.05) {
        // Price above mean: sell
        order_book_.addOrder(Order(order_id_++, Order::Side::SELL, price - 0.01, qty_, ts, Order::OrderType::LIMIT));
    }
} 