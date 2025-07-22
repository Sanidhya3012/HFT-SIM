#include <iostream>
#include "Order.h"
#include "OrderBook.h"
#include "CSVParser.h"
#include "TradeLogger.h"

int main() {
    OrderBook ob;
    TradeLogger logger("../data/trades.csv");
    ob.setTradeLogger(&logger);

    // Example: Load orders from CSV file
    std::string filename = "../data/orders.csv";
    auto orders = parseOrdersFromCSV(filename);
    std::cout << "Loaded " << orders.size() << " orders from CSV." << std::endl;
    for (const auto& order : orders) {
        ob.addOrder(order);
    }

    std::cout << "\nMatching orders..." << std::endl;
    ob.matchOrders();

    // Add and then cancel an order
    ob.addOrder(Order(5, Order::Side::BUY, 100.5, 12, 1625158804));
    std::cout << "\nCanceling order 5..." << std::endl;
    ob.cancelOrder(5);

    // Print remaining buy orders
    std::cout << "\nRemaining Buy Orders:" << std::endl;
    for (const auto& order : ob.getBuyOrders()) {
        std::cout << "OrderID: " << order.getOrderID() << ", Price: " << order.getPrice() << ", Qty: " << order.getQuantity() << std::endl;
    }
    // Print remaining sell orders
    std::cout << "\nRemaining Sell Orders:" << std::endl;
    for (const auto& order : ob.getSellOrders()) {
        std::cout << "OrderID: " << order.getOrderID() << ", Price: " << order.getPrice() << ", Qty: " << order.getQuantity() << std::endl;
    }

    // Print trade summary
    logger.printSummary();
    return 0;
} 