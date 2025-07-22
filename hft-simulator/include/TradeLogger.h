#ifndef TRADELOGGER_H
#define TRADELOGGER_H

#include <string>
#include <vector>
#include <fstream>
#include <cstdint>

// Trade struct represents a matched trade between a buy and sell order
struct Trade {
    int buy_order_id;
    int sell_order_id;
    double price;
    int quantity;
    std::uint64_t timestamp;
};

// TradeLogger logs trades to a CSV file and provides summary statistics
class TradeLogger {
public:
    // Create a logger that writes to the given CSV file
    TradeLogger(const std::string& filename);
    ~TradeLogger();

    // Log a trade to CSV and store in memory
    void logTrade(const Trade& trade);

    // Output summary: total trades, volume, and P&L
    void printSummary() const;

private:
    std::ofstream file_;
    std::vector<Trade> trades_;
};

#endif // TRADELOGGER_H 