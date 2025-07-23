#include "TradeLogger.h"
#include <iostream>
#include <iomanip>
#include <ctime>
#include "Utils.h"

TradeLogger::TradeLogger(const std::string& filename) : file_(filename) {
    // Write CSV header
    file_ << "buy_order_id,sell_order_id,price,quantity,timestamp,aggressor_side,realized_pnl,net_position,avg_price\n";
}

TradeLogger::~TradeLogger() {
    if (file_.is_open()) {
        writePnLSummary();
        file_.close();
    }
}

void TradeLogger::logTrade(const Trade& trade) {
    trades_.push_back(trade);
    updatePosition(trade);
    
    if (file_.is_open()) {
        file_ << trade.buy_order_id << ','
              << trade.sell_order_id << ','
              << std::fixed << std::setprecision(2) << trade.price << ','
              << trade.quantity << ','
              << trade.timestamp << ','
              << trade.aggressor_side << ','
              << std::fixed << std::setprecision(2) << position_.realized_pnl << ','
              << position_.net_quantity << ','
              << std::fixed << std::setprecision(2) << position_.average_price << '\n';
    }
}

void TradeLogger::updatePosition(const Trade& trade) {
    bool is_buy = (trade.aggressor_side == "BUY");
    position_.update(trade.quantity, trade.price, is_buy);
    last_mark_price_ = trade.price;
    position_.mark_to_market(last_mark_price_);
}

void TradeLogger::updateMarkPrice(double price) {
    last_mark_price_ = price;
    position_.mark_to_market(price);
}

double TradeLogger::getAggressorBasedPnL() const {
    double pnl = 0.0;
    for (const auto& trade : trades_) {
        if (trade.aggressor_side == "BUY") {
            pnl -= trade.price * trade.quantity;
        } else {
            pnl += trade.price * trade.quantity;
        }
    }
    return pnl;
}

double TradeLogger::getRealizedPnL() const {
    return position_.realized_pnl;
}

double TradeLogger::getUnrealizedPnL() const {
    return position_.unrealized_pnl;
}

double TradeLogger::getTotalPnL() const {
    return position_.realized_pnl + position_.unrealized_pnl;
}

int TradeLogger::getNetPosition() const {
    return position_.net_quantity;
}

double TradeLogger::getAveragePrice() const {
    return position_.average_price;
}

void TradeLogger::writePnLSummary() {
    file_ << "\nTrading Summary\n";
    file_ << "Aggressor-Based P&L," << std::fixed << std::setprecision(2) << getAggressorBasedPnL() << "\n";
    file_ << "Realized P&L," << position_.realized_pnl << "\n";
    file_ << "Unrealized P&L," << position_.unrealized_pnl << "\n";
    file_ << "Total P&L," << getTotalPnL() << "\n";
    file_ << "Final Position," << position_.net_quantity << "\n";
    file_ << "Average Price," << position_.average_price << "\n";
    file_ << "Mark Price," << last_mark_price_ << "\n";
}

void TradeLogger::printSummary() const {
    std::cout << "\n=== Trade Summary ===\n";
    std::cout << "Total Trades: " << trades_.size() << "\n";
    std::cout << "Aggressor-Based P&L: " << std::fixed << std::setprecision(2) << getAggressorBasedPnL() << "\n";
    std::cout << "Realized P&L: " << position_.realized_pnl << "\n";
    std::cout << "Unrealized P&L: " << position_.unrealized_pnl << "\n";
    std::cout << "Total P&L: " << getTotalPnL() << "\n";
    std::cout << "Final Position: " << position_.net_quantity << "\n";
    std::cout << "Average Price: " << position_.average_price << "\n";
    std::cout << "Mark Price: " << last_mark_price_ << "\n";
} 