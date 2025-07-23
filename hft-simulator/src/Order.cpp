#include "Order.h"

Order::Order(int order_id, Side side, double price, int quantity, std::uint64_t timestamp, OrderType type)
    : order_id_(order_id), side_(side), price_(price), quantity_(quantity), timestamp_(timestamp), type_(type), stop_price_(0.0) {}

Order::Order(int order_id, Side side, double price, int quantity, std::uint64_t timestamp, double stop_price)
    : order_id_(order_id), side_(side), price_(price), quantity_(quantity), timestamp_(timestamp), type_(OrderType::STOP), stop_price_(stop_price) {}

// Accessors
int Order::getOrderID() const { return order_id_; }
Order::Side Order::getSide() const { return side_; }
double Order::getPrice() const { return price_; }
int Order::getQuantity() const { return quantity_; }
std::uint64_t Order::getTimestamp() const { return timestamp_; }
Order::OrderType Order::getOrderType() const { return type_; }
double Order::getStopPrice() const { return stop_price_; }

// Mutators
void Order::setOrderID(int order_id) { order_id_ = order_id; }
void Order::setSide(Side side) { side_ = side; }
void Order::setPrice(double price) { price_ = price; }
void Order::setQuantity(int quantity) { quantity_ = quantity; }
void Order::setTimestamp(std::uint64_t timestamp) { timestamp_ = timestamp; }
void Order::setOrderType(OrderType type) { type_ = type; }
void Order::setStopPrice(double stop_price) { stop_price_ = stop_price; }

// Utility
std::string Order::sideToString(Side side) {
    return (side == Side::BUY) ? "BUY" : "SELL";
}
std::string Order::typeToString(OrderType type) {
    switch (type) {
        case OrderType::LIMIT: return "LIMIT";
        case OrderType::MARKET: return "MARKET";
        case OrderType::STOP: return "STOP";
        default: return "UNKNOWN";
    }
} 