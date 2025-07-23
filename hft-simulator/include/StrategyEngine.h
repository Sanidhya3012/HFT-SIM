#ifndef STRATEGYENGINE_H
#define STRATEGYENGINE_H

#include "OrderBook.h"
#include <atomic>
#include <chrono>
#include <vector>
#include <memory>

// Base class for all strategies
class Strategy {
public:
    virtual ~Strategy() {}
    virtual void step() = 0; // Called each simulation tick
};

// Market Making Strategy: Quotes both bid and ask around mid-price
class MarketMakingStrategy : public Strategy {
public:
    MarketMakingStrategy(OrderBook& ob, double spread, int qty)
        : order_book_(ob), spread_(spread), qty_(qty), order_id_(10000) {}
    void step() override;
private:
    OrderBook& order_book_;
    double spread_;
    int qty_;
    int order_id_;
};

// Momentum Trading Strategy: Goes with short-term price trends
class MomentumStrategy : public Strategy {
public:
    MomentumStrategy(OrderBook& ob, int qty)
        : order_book_(ob), qty_(qty), order_id_(20000), last_price_(0.0) {}
    void step() override;
private:
    OrderBook& order_book_;
    int qty_;
    int order_id_;
    double last_price_;
};

// Mean Reversion Strategy: Bets on return to mean using moving average
class MeanReversionStrategy : public Strategy {
public:
    MeanReversionStrategy(OrderBook& ob, int qty, int window)
        : order_book_(ob), qty_(qty), window_(window), order_id_(30000) {}
    void step() override;
private:
    OrderBook& order_book_;
    int qty_;
    int window_;
    int order_id_;
    std::vector<double> price_history_;
};

// StrategyEngine: Runs multiple strategies in sequence
class StrategyEngine {
public:
    StrategyEngine(OrderBook& order_book, double spread, int interval_ms);
    ~StrategyEngine();
    
    void start();
    void stop();
    bool isRunning() const;
    void run();

private:
    OrderBook& order_book_;
    double spread_;
    int interval_ms_;
    bool running_;
    int next_order_id_;
};

#endif // STRATEGYENGINE_H 