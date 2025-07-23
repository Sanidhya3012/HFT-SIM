#pragma once
#include <string>
#include <fstream>
#include <vector>
#include <map>

struct Trade {
    int buy_order_id;
    int sell_order_id;
    double price;
    int quantity;
    std::string timestamp;
    std::string aggressor_side;
};

struct Position {
    int net_quantity = 0;
    double average_price = 0.0;
    double realized_pnl = 0.0;
    double unrealized_pnl = 0.0;
    
    void update(int qty, double price, bool is_buy) {
        if (is_buy) {
            if (net_quantity < 0) { // Covering short position
                int cover_qty = std::min(qty, -net_quantity);
                realized_pnl += (average_price - price) * cover_qty;
                qty -= cover_qty;
                net_quantity += cover_qty;
            }
            if (qty > 0) { // Building long position
                double new_total = (average_price * net_quantity) + (price * qty);
                net_quantity += qty;
                average_price = new_total / net_quantity;
            }
        } else { // Selling
            if (net_quantity > 0) { // Closing long position
                int close_qty = std::min(qty, net_quantity);
                realized_pnl += (price - average_price) * close_qty;
                qty -= close_qty;
                net_quantity -= close_qty;
            }
            if (qty > 0) { // Building short position
                double new_total = (average_price * -net_quantity) + (price * qty);
                net_quantity -= qty;
                average_price = new_total / -net_quantity;
            }
        }
    }
    
    void mark_to_market(double mark_price) {
        if (net_quantity > 0) {
            unrealized_pnl = (mark_price - average_price) * net_quantity;
        } else if (net_quantity < 0) {
            unrealized_pnl = (average_price - mark_price) * -net_quantity;
        } else {
            unrealized_pnl = 0.0;
        }
    }
};

class TradeLogger {
public:
    TradeLogger(const std::string& filename);
    ~TradeLogger();
    
    void logTrade(const Trade& trade);
    void printSummary() const;
    
    // New methods for improved P&L tracking
    double getAggressorBasedPnL() const;
    double getRealizedPnL() const;
    double getUnrealizedPnL() const;
    double getTotalPnL() const;
    int getNetPosition() const;
    double getAveragePrice() const;
    void updateMarkPrice(double price);

private:
    std::ofstream file_;
    std::vector<Trade> trades_;
    Position position_;
    double last_mark_price_ = 0.0;
    
    // Helper methods
    void writePnLSummary();
    void updatePosition(const Trade& trade);
}; 