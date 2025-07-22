#include "Order.h"

Order::Order(int order_id, Side side, double price, int quantity, std::uint64_t timestamp)
    : order_id_(order_id), side_(side), price_(price), quantity_(quantity), timestamp_(timestamp) {}

// Accessors
int Order::getOrderID() const { return order_id_; }
Order::Side Order::getSide() const { return side_; }
double Order::getPrice() const { return price_; }
int Order::getQuantity() const { return quantity_; }
std::uint64_t Order::getTimestamp() const { return timestamp_; }

// Mutators
void Order::setOrderID(int order_id) { order_id_ = order_id; }
void Order::setSide(Side side) { side_ = side; }
void Order::setPrice(double price) { price_ = price; }
void Order::setQuantity(int quantity) { quantity_ = quantity; }
void Order::setTimestamp(std::uint64_t timestamp) { timestamp_ = timestamp; }

// Utility
std::string Order::sideToString(Side side) {
    return (side == Side::BUY) ? "BUY" : "SELL";
} 