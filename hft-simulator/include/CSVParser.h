#ifndef CSVPARSER_H
#define CSVPARSER_H

#include "Order.h"
#include <vector>
#include <string>

// Parses a CSV file with columns: order_id, side, price, quantity, timestamp
// Returns a vector of Order objects for use in the order book
std::vector<Order> parseOrdersFromCSV(const std::string& filename);

#endif // CSVPARSER_H 