#include "CSVParser.h"
#include <fstream>
#include <sstream>
#include <iostream>

std::vector<Order> parseOrdersFromCSV(const std::string& filename) {
    std::vector<Order> orders;
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return orders;
    }
    std::string line;
    // Skip header
    if (!std::getline(file, line)) return orders;
    while (std::getline(file, line)) {
        std::istringstream ss(line);
        std::string token;
        int order_id, quantity;
        std::string side_str;
        double price;
        std::uint64_t timestamp;
        // Parse order_id
        if (!std::getline(ss, token, ',')) continue;
        order_id = std::stoi(token);
        // Parse side
        if (!std::getline(ss, token, ',')) continue;
        side_str = token;
        Order::Side side = (side_str == "buy" || side_str == "BUY") ? Order::Side::BUY : Order::Side::SELL;
        // Parse price
        if (!std::getline(ss, token, ',')) continue;
        price = std::stod(token);
        // Parse quantity
        if (!std::getline(ss, token, ',')) continue;
        quantity = std::stoi(token);
        // Parse timestamp
        if (!std::getline(ss, token, ',')) continue;
        timestamp = std::stoull(token);
        orders.emplace_back(order_id, side, price, quantity, timestamp);
    }
    return orders;
} 