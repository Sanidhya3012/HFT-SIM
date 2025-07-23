#include <iostream>
#include <memory>
#include "Order.h"
#include "OrderBook.h"
#include "CSVParser.h"
#include "TradeLogger.h"
#include "StrategyEngine.h"

int main() {
    OrderBook ob;
    TradeLogger logger("../data/trades.csv");
    ob.setTradeLogger(&logger);

    // Load initial orders from CSV
    std::string filename = "../data/orders.csv";
    auto orders = parseOrdersFromCSV(filename);
    std::cout << "Loaded " << orders.size() << " orders from CSV." << std::endl;
    for (const auto& order : orders) {
        ob.addOrder(order);
    }
    ob.matchOrders();
    ob.checkStopOrders();

    // Create strategy engine with market making parameters
    StrategyEngine engine(ob, 0.5, 100); // 0.5 spread, 100ms interval
    
    // Start the engine (no-op in non-threaded mode)
    engine.start();
    
    // Run for a while
    for (int i = 0; i < 50; ++i) {
        engine.run();
        ob.matchOrders();
        ob.checkStopOrders();
    }
    
    // Stop the engine (no-op in non-threaded mode)
    engine.stop();

    // Print remaining orders
    std::cout << "\nRemaining Buy Orders:" << std::endl;
    for (const auto& order : ob.getBuyOrders()) {
        std::cout << "OrderID: " << order.getOrderID() << ", Price: " << order.getPrice() << ", Qty: " << order.getQuantity() << std::endl;
    }
    std::cout << "\nRemaining Sell Orders:" << std::endl;
    for (const auto& order : ob.getSellOrders()) {
        std::cout << "OrderID: " << order.getOrderID() << ", Price: " << order.getPrice() << ", Qty: " << order.getQuantity() << std::endl;
    }

    // Print trade summary
    logger.printSummary();
    return 0;
} 