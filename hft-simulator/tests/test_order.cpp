#include "Order.h"
#include <cassert>
#include <iostream>

void test_order_constructor_and_accessors() {
    Order o(42, Order::Side::SELL, 123.45, 100, 999999);
    assert(o.getOrderID() == 42);
    assert(o.getSide() == Order::Side::SELL);
    assert(o.getPrice() == 123.45);
    assert(o.getQuantity() == 100);
    assert(o.getTimestamp() == 999999);
}

void test_order_mutators() {
    Order o(1, Order::Side::BUY, 1.0, 1, 1);
    o.setOrderID(2);
    o.setSide(Order::Side::SELL);
    o.setPrice(2.5);
    o.setQuantity(10);
    o.setTimestamp(123456);
    assert(o.getOrderID() == 2);
    assert(o.getSide() == Order::Side::SELL);
    assert(o.getPrice() == 2.5);
    assert(o.getQuantity() == 10);
    assert(o.getTimestamp() == 123456);
}

void test_order_sideToString() {
    assert(Order::sideToString(Order::Side::BUY) == "BUY");
    assert(Order::sideToString(Order::Side::SELL) == "SELL");
}

int main() {
    test_order_constructor_and_accessors();
    test_order_mutators();
    test_order_sideToString();
    std::cout << "Order class tests passed!\n";
    return 0;
} 