#ifndef ORDER_H
#define ORDER_H

#include <string>
#include <cstdint>

// Represents a market order (buy or sell) with price, quantity, and timestamp
class Order {
public:
    enum class Side { BUY, SELL };

    // Construct an order with all fields
    Order(int order_id, Side side, double price, int quantity, std::uint64_t timestamp);

    // Accessors (getters)
    int getOrderID() const;
    Side getSide() const;
    double getPrice() const;
    int getQuantity() const;
    std::uint64_t getTimestamp() const;

    // Mutators (setters)
    void setOrderID(int order_id);
    void setSide(Side side);
    void setPrice(double price);
    void setQuantity(int quantity);
    void setTimestamp(std::uint64_t timestamp);

    // Convert Side enum to string ("BUY" or "SELL")
    static std::string sideToString(Side side);

private:
    int order_id_;
    Side side_;
    double price_;
    int quantity_;
    std::uint64_t timestamp_;
};

#endif // ORDER_H 