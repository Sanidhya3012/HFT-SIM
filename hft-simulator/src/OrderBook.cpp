#include "OrderBook.h"
#include <iostream>
#include <random>
#include <chrono>
#include "TradeLogger.h"
#include "Utils.h"

OrderBook::OrderBook() {}

// Add a new order to the book and update lookup
void OrderBook::addOrder(const Order& order) {
    if (order.getOrderType() == Order::OrderType::MARKET) {
        addMarketOrder(order);
        return;
    } else if (order.getOrderType() == Order::OrderType::STOP) {
        addStopOrder(order);
        return;
    }
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

// Add a market order: match immediately at best price
void OrderBook::addMarketOrder(const Order& order) {
    // Market order: match with best available price until filled or book empty
    int remaining_qty = order.getQuantity();
    if (order.getSide() == Order::Side::BUY) {
        while (remaining_qty > 0 && !sell_orders_.empty()) {
            double best_sell = sell_price_pq_.top();
            if (sell_orders_.find(best_sell) == sell_orders_.end()) {
                sell_price_pq_.pop();
                continue;
            }
            std::list<Order>& sell_list = sell_orders_[best_sell];
            Order& sell_order = sell_list.front();
            int trade_qty = std::min(remaining_qty, sell_order.getQuantity());
            double trade_price = best_sell;
            std::cout << "[MarketOrder] BuyOrder " << order.getOrderID() << " & SellOrder " << sell_order.getOrderID()
                      << ", Qty: " << trade_qty << ", Price: " << trade_price << std::endl;
            if (trade_logger_) {
                Trade trade;
                trade.buy_order_id = order.getOrderID();
                trade.sell_order_id = sell_order.getOrderID();
                trade.price = trade_price;
                trade.quantity = trade_qty;
                std::uint64_t ts = static_cast<std::uint64_t>(std::chrono::system_clock::now().time_since_epoch().count());
                trade.timestamp = formatTimestamp(ts);
                trade.aggressor_side = Order::sideToString(order.getSide());
                trade_logger_->logTrade(trade);
            }
            remaining_qty -= trade_qty;
            sell_order.setQuantity(sell_order.getQuantity() - trade_qty);
            if (sell_order.getQuantity() == 0) {
                removeOrder(sell_order.getOrderID());
            }
        }
    } else { // SELL market order
        while (remaining_qty > 0 && !buy_orders_.empty()) {
            double best_buy = buy_price_pq_.top();
            if (buy_orders_.find(best_buy) == buy_orders_.end()) {
                buy_price_pq_.pop();
                continue;
            }
            std::list<Order>& buy_list = buy_orders_[best_buy];
            Order& buy_order = buy_list.front();
            int trade_qty = std::min(remaining_qty, buy_order.getQuantity());
            double trade_price = best_buy;
            std::cout << "[MarketOrder] BuyOrder " << buy_order.getOrderID() << " & SellOrder " << order.getOrderID()
                      << ", Qty: " << trade_qty << ", Price: " << trade_price << std::endl;
            if (trade_logger_) {
                Trade trade;
                trade.buy_order_id = buy_order.getOrderID();
                trade.sell_order_id = order.getOrderID();
                trade.price = trade_price;
                trade.quantity = trade_qty;
                std::uint64_t ts = static_cast<std::uint64_t>(std::chrono::system_clock::now().time_since_epoch().count());
                trade.timestamp = formatTimestamp(ts);
                trade.aggressor_side = Order::sideToString(order.getSide());
                trade_logger_->logTrade(trade);
            }
            remaining_qty -= trade_qty;
            buy_order.setQuantity(buy_order.getQuantity() - trade_qty);
            if (buy_order.getQuantity() == 0) {
                removeOrder(buy_order.getOrderID());
            }
        }
    }
    // If remaining_qty > 0, market order is not fully filled and remainder is dropped
}

// Add a stop order: store until activation
void OrderBook::addStopOrder(const Order& order) {
    if (order.getSide() == Order::Side::BUY) {
        stop_buy_orders_.push_back(order);
    } else {
        stop_sell_orders_.push_back(order);
    }
}

// Check and activate stop orders if price is reached
void OrderBook::checkStopOrders() {
    // Activate buy stop orders if best sell <= stop price
    if (!sell_orders_.empty()) {
        double best_sell = sell_price_pq_.empty() ? 0.0 : sell_price_pq_.top();
        std::vector<Order> still_pending;
        for (size_t i = 0; i < stop_buy_orders_.size(); ++i) {
            const Order& o = stop_buy_orders_[i];
            if (!sell_orders_.empty() && best_sell <= o.getStopPrice()) {
                // Activate as market order
                Order market_order(o.getOrderID(), o.getSide(), 0.0, o.getQuantity(), o.getTimestamp(), Order::OrderType::MARKET);
                addMarketOrder(market_order);
            } else {
                still_pending.push_back(o);
            }
        }
        stop_buy_orders_ = still_pending;
    }
    // Activate sell stop orders if best buy >= stop price
    if (!buy_orders_.empty()) {
        double best_buy = buy_price_pq_.empty() ? 0.0 : buy_price_pq_.top();
        std::vector<Order> still_pending;
        for (size_t i = 0; i < stop_sell_orders_.size(); ++i) {
            const Order& o = stop_sell_orders_[i];
            if (!buy_orders_.empty() && best_buy >= o.getStopPrice()) {
                // Activate as market order
                Order market_order(o.getOrderID(), o.getSide(), 0.0, o.getQuantity(), o.getTimestamp(), Order::OrderType::MARKET);
                addMarketOrder(market_order);
            } else {
                still_pending.push_back(o);
            }
        }
        stop_sell_orders_ = still_pending;
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
            std::uint64_t ts = static_cast<std::uint64_t>(std::chrono::system_clock::now().time_since_epoch().count());
            trade.timestamp = formatTimestamp(ts);
            // Aggressor is the order that arrived last (here, sell_order if matching buy, buy_order if matching sell)
            trade.aggressor_side = (buy_order.getTimestamp() > sell_order.getTimestamp()) ? "BUY" : "SELL";
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