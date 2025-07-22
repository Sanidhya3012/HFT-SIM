#ifndef ORDERBOOK_H
#define ORDERBOOK_H

#include "Order.h"
#include "TradeLogger.h"
#include <map>
#include <list>
#include <unordered_map>
#include <vector>
#include <queue>

// OrderBook manages buy and sell orders, supports add, match, and cancel operations.
class OrderBook {
public:
    OrderBook();

    // Add a new order to the book
    void addOrder(const Order& order);

    // Attempt to match orders (buy vs sell). Matches best prices and logs trades if logger is set.
    void matchOrders();

    // Cancel an order by ID. Returns true if canceled, false if not found.
    bool cancelOrder(int order_id);

    // Get all current buy orders (for inspection/testing)
    std::vector<Order> getBuyOrders() const;
    // Get all current sell orders (for inspection/testing)
    std::vector<Order> getSellOrders() const;

    // Set the trade logger for recording matched trades
    void setTradeLogger(TradeLogger* logger);

private:
    // Buy orders: price descending (highest first)
    std::map<double, std::list<Order>, std::greater<double>> buy_orders_;
    // Sell orders: price ascending (lowest first)
    std::map<double, std::list<Order>> sell_orders_;
    // Fast lookup for canceling orders by ID
    std::unordered_map<int, std::pair<Order::Side, std::pair<double, std::list<Order>::iterator>>> order_lookup_;

    TradeLogger* trade_logger_ = nullptr;

    // Priority queues for fast access to best prices
    // These are updated on add/cancel for O(1) best price lookup
    std::priority_queue<double> buy_price_pq_; // max-heap for buy prices
    std::priority_queue<double, std::vector<double>, std::greater<double>> sell_price_pq_; // min-heap for sell prices

    // Helper to remove order from book and lookup
    void removeOrder(int order_id);

    // Helper to maintain price priority queues
    void updatePriceQueues();
};

#endif // ORDERBOOK_H 