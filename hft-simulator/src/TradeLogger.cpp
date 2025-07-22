#include "TradeLogger.h"
#include <iostream>
#include <iomanip>

TradeLogger::TradeLogger(const std::string& filename) : file_(filename) {
    // Write CSV header
    file_ << "buy_order_id,sell_order_id,price,quantity,timestamp\n";
}

TradeLogger::~TradeLogger() {
    if (file_.is_open()) {
        file_.close();
    }
}

void TradeLogger::logTrade(const Trade& trade) {
    trades_.push_back(trade);
    if (file_.is_open()) {
        file_ << trade.buy_order_id << ','
              << trade.sell_order_id << ','
              << std::fixed << std::setprecision(2) << trade.price << ','
              << trade.quantity << ','
              << trade.timestamp << '\n';
    }
}

void TradeLogger::printSummary() const {
    int total_trades = trades_.size();
    int total_volume = 0;
    double pnl = 0.0;
    // For P&L, assume market maker is both buyer and seller, so P&L is sum(sell - buy) * qty
    // Here, we just sum (sell - buy) for each trade if we can infer direction
    for (const auto& trade : trades_) {
        total_volume += trade.quantity;
        // For demonstration, treat buy_order_id < sell_order_id as market maker buying, else selling
        // In a real system, you'd track which side is the market maker
        if (trade.buy_order_id < trade.sell_order_id) {
            pnl -= trade.price * trade.quantity; // bought
        } else {
            pnl += trade.price * trade.quantity; // sold
        }
    }
    std::cout << "\n--- Trade Summary ---" << std::endl;
    std::cout << "Total Trades: " << total_trades << std::endl;
    std::cout << "Total Volume: " << total_volume << std::endl;
    std::cout << "P&L: " << std::fixed << std::setprecision(2) << pnl << std::endl;
} 