#include "OrderBook.h"
#include <iostream>
#include <random>
#include <chrono>

OrderBook::OrderBook() {}

// Add a new order to the book and update lookup
void OrderBook::addOrder(const Order& order) {
    if (order.getSide() == Order::Side::BUY) {
        buy_orders_[order.getPrice()].push_back(order);
        std::list<Order>::iterator it = --buy_orders_[order.getPrice()].end();
        order_lookup_[order.getOrderID()] = std::make_pair(order.getSide(), std::make_pair(order.getPrice(), it));
        buy_price_pq_.push(order.getPrice());
    } else {
        sell_orders_[order.getPrice()].push_back(order);
        std::list<Order>::iterator it = --sell_orders_[order.getPrice()].end();
        order_lookup_[order.getOrderID()] = std::make_pair(order.getSide(), std::make_pair(order.getPrice(), it));
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
        std::list<Order>& buy_list = buy_orders_[best_buy];
        std::list<Order>& sell_list = sell_orders_[best_sell];
        Order& buy_order = buy_list.front();
        Order& sell_order = sell_list.front();
        int trade_qty = std::min(buy_order.getQuantity(), sell_order.getQuantity());
        double trade_price = best_sell; // Use sell price for trade
        std::cout << "Trade: BuyOrder " << buy_order.getOrderID() << " & SellOrder " << sell_order.getOrderID()
                  << ", Qty: " << trade_qty << ", Price: " << trade_price << std::endl;
        if (trade_logger_) {
            Trade trade;
            trade.buy_order_id = buy_order.getOrderID();
            trade.sell_order_id = sell_order.getOrderID();
            trade.price = trade_price;
            trade.quantity = trade_qty;
            trade.timestamp = static_cast<std::uint64_t>(std::chrono::system_clock::now().time_since_epoch().count());
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
    std::unordered_map<int, std::pair<Order::Side, std::pair<double, std::list<Order>::iterator> > >::iterator it = order_lookup_.find(order_id);
    if (it == order_lookup_.end()) return false;
    removeOrder(order_id);
    return true;
}

// Remove order from book and lookup
void OrderBook::removeOrder(int order_id) {
    std::unordered_map<int, std::pair<Order::Side, std::pair<double, std::list<Order>::iterator> > >::iterator it = order_lookup_.find(order_id);
    if (it == order_lookup_.end()) return;
    Order::Side side = it->second.first;
    double price = it->second.second.first;
    std::list<Order>::iterator list_it = it->second.second.second;
    if (side == Order::Side::BUY) {
        std::map<double, std::list<Order>, std::greater<double> >::iterator price_it = buy_orders_.find(price);
        if (price_it != buy_orders_.end()) {
            price_it->second.erase(list_it);
            if (price_it->second.empty()) {
                buy_orders_.erase(price_it);
                // Note: Priority queues may contain stale prices; we skip them in matchOrders
            }
        }
    } else {
        std::map<double, std::list<Order> >::iterator price_it = sell_orders_.find(price);
        if (price_it != sell_orders_.end()) {
            price_it->second.erase(list_it);
            if (price_it->second.empty()) {
                sell_orders_.erase(price_it);
                // Note: Priority queues may contain stale prices; we skip them in matchOrders
            }
        }
    }
    order_lookup_.erase(it);
}

// Get all current buy orders
std::vector<Order> OrderBook::getBuyOrders() const {
    std::vector<Order> result;
    for (std::map<double, std::list<Order>, std::greater<double> >::const_iterator it = buy_orders_.begin(); it != buy_orders_.end(); ++it) {
        double price = it->first;
        const std::list<Order>& orders = it->second;
        for (std::list<Order>::const_iterator oit = orders.begin(); oit != orders.end(); ++oit) {
            result.push_back(*oit);
        }
    }
    return result;
}

// Get all current sell orders
std::vector<Order> OrderBook::getSellOrders() const {
    std::vector<Order> result;
    for (std::map<double, std::list<Order> >::const_iterator it = sell_orders_.begin(); it != sell_orders_.end(); ++it) {
        double price = it->first;
        const std::list<Order>& orders = it->second;
        for (std::list<Order>::const_iterator oit = orders.begin(); oit != orders.end(); ++oit) {
            result.push_back(*oit);
        }
    }
    return result;
}

void OrderBook::setTradeLogger(TradeLogger* logger) {
    trade_logger_ = logger;
} 