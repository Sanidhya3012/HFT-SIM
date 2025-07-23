#ifndef ORDER_H
#define ORDER_H

#include <string>
#include <cstdint>

class Order {
public:
    enum class Side { BUY, SELL };
    enum class OrderType { LIMIT, MARKET, STOP };

    // Constructor for limit/market orders
    Order(int order_id, Side side, double price, int quantity, std::uint64_t timestamp, OrderType type = OrderType::LIMIT);
    // Constructor for stop orders
    Order(int order_id, Side side, double price, int quantity, std::uint64_t timestamp, double stop_price);

    // Accessors
    int getOrderID() const;
    Side getSide() const;
    double getPrice() const;
    int getQuantity() const;
    std::uint64_t getTimestamp() const;
    OrderType getOrderType() const;
    double getStopPrice() const;

    // Mutators
    void setOrderID(int order_id);
    void setSide(Side side);
    void setPrice(double price);
    void setQuantity(int quantity);
    void setTimestamp(std::uint64_t timestamp);
    void setOrderType(OrderType type);
    void setStopPrice(double stop_price);

    // Utility
    static std::string sideToString(Side side);
    static std::string typeToString(OrderType type);

private:
    int order_id_;
    Side side_;
    double price_;
    int quantity_;
    std::uint64_t timestamp_;
    OrderType type_;
    double stop_price_; // Only used for STOP orders
};

#endif // ORDER_H 