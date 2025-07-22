#include "OrderBook.h"
#include <iostream>

OrderBook::OrderBook() {}

// Add a new order to the book and update lookup
void OrderBook::addOrder(const Order& order) {
    auto& book = (order.getSide() == Order::Side::BUY) ? buy_orders_ : sell_orders_;
    book[order.getPrice()].push_back(order);
    auto it = --book[order.getPrice()].end();
    order_lookup_[order.getOrderID()] = {order.getSide(), {order.getPrice(), it}};
    // Update price priority queues for O(1) best price lookup
    if (order.getSide() == Order::Side::BUY) {
        buy_price_pq_.push(order.getPrice());
    } else {
        sell_price_pq_.push(order.getPrice());
    }
}

// Attempt to match top buy and sell orders using priority queues for best price lookup
void OrderBook::matchOrders() {
    // Performance optimization: Use priority queues for O(1) best price lookup
    // This avoids O(log N) map iteration for best price on each match
    while (!buy_orders_.empty() && !sell_orders_.empty() &&
           !buy_price_pq_.empty() && !sell_price_pq_.empty()) {
        double best_buy = buy_price_pq_.top();
        double best_sell = sell_price_pq_.top();
        // Remove stale prices (no orders at this price)
        if (buy_orders_.find(best_buy) == buy_orders_.end()) {
            buy_price_pq_.pop();
            continue;
        }
        if (sell_orders_.find(best_sell) == sell_orders_.end()) {
            sell_price_pq_.pop();
            continue;
        }
        if (best_buy < best_sell) break; // No match possible
        auto& buy_list = buy_orders_[best_buy];
        auto& sell_list = sell_orders_[best_sell];
        Order& buy_order = buy_list.front();
        Order& sell_order = sell_list.front();
        int trade_qty = std::min(buy_order.getQuantity(), sell_order.getQuantity());
        double trade_price = best_sell; // Use sell price for trade
        std::cout << "Trade: BuyOrder " << buy_order.getOrderID() << " & SellOrder " << sell_order.getOrderID()
                  << ", Qty: " << trade_qty << ", Price: " << trade_price << std::endl;
        if (trade_logger_) {
            Trade trade{buy_order.getOrderID(), sell_order.getOrderID(), trade_price, trade_qty, static_cast<std::uint64_t>(std::chrono::system_clock::now().time_since_epoch().count())};
            trade_logger_->logTrade(trade);
        }
        buy_order.setQuantity(buy_order.getQuantity() - trade_qty);
        sell_order.setQuantity(sell_order.getQuantity() - trade_qty);
        if (buy_order.getQuantity() == 0) {
            removeOrder(buy_order.getOrderID());
        }
        if (sell_order.getQuantity() == 0) {
            removeOrder(sell_order.getOrderID());
        }
    }
}

// Cancel an order by ID
bool OrderBook::cancelOrder(int order_id) {
    auto it = order_lookup_.find(order_id);
    if (it == order_lookup_.end()) return false;
    removeOrder(order_id);
    return true;
}

// Remove order from book and lookup
void OrderBook::removeOrder(int order_id) {
    auto it = order_lookup_.find(order_id);
    if (it == order_lookup_.end()) return;
    Order::Side side = it->second.first;
    double price = it->second.second.first;
    auto list_it = it->second.second.second;
    auto& book = (side == Order::Side::BUY) ? buy_orders_ : sell_orders_;
    auto price_it = book.find(price);
    if (price_it != book.end()) {
        price_it->second.erase(list_it);
        if (price_it->second.empty()) {
            book.erase(price_it);
            // Note: Priority queues may contain stale prices; we skip them in matchOrders
        }
    }
    order_lookup_.erase(it);
}

// Get all current buy orders
std::vector<Order> OrderBook::getBuyOrders() const {
    std::vector<Order> result;
    for (const auto& [price, orders] : buy_orders_) {
        for (const auto& order : orders) {
            result.push_back(order);
        }
    }
    return result;
}

// Get all current sell orders
std::vector<Order> OrderBook::getSellOrders() const {
    std::vector<Order> result;
    for (const auto& [price, orders] : sell_orders_) {
        for (const auto& order : orders) {
            result.push_back(order);
        }
    }
    return result;
}

void OrderBook::setTradeLogger(TradeLogger* logger) {
    trade_logger_ = logger;
} 