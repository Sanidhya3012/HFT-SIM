#include "OrderBook.h"
#include <cassert>
#include <iostream>

void test_add_and_cancel_order() {
    OrderBook ob;
    Order buy(1, Order::Side::BUY, 100.0, 10, 1);
    ob.addOrder(buy);
    assert(ob.getBuyOrders().size() == 1);
    assert(ob.getSellOrders().empty());
    assert(ob.cancelOrder(1));
    assert(ob.getBuyOrders().empty());
    // Cancel non-existent order
    assert(!ob.cancelOrder(999));
}

void test_match_orders_full_fill() {
    OrderBook ob;
    Order buy(1, Order::Side::BUY, 101.0, 10, 1);
    Order sell(2, Order::Side::SELL, 100.0, 10, 2);
    ob.addOrder(buy);
    ob.addOrder(sell);
    ob.matchOrders();
    // Both orders should be filled and removed
    assert(ob.getBuyOrders().empty());
    assert(ob.getSellOrders().empty());
}

void test_match_orders_partial_fill() {
    OrderBook ob;
    Order buy(1, Order::Side::BUY, 101.0, 5, 1);
    Order sell(2, Order::Side::SELL, 100.0, 10, 2);
    ob.addOrder(buy);
    ob.addOrder(sell);
    ob.matchOrders();
    // Buy order filled, sell order partially filled
    assert(ob.getBuyOrders().empty());
    auto sells = ob.getSellOrders();
    assert(sells.size() == 1);
    assert(sells[0].getQuantity() == 5);
}

void test_match_orders_no_match() {
    OrderBook ob;
    Order buy(1, Order::Side::BUY, 99.0, 10, 1);
    Order sell(2, Order::Side::SELL, 100.0, 10, 2);
    ob.addOrder(buy);
    ob.addOrder(sell);
    ob.matchOrders();
    // No match should occur
    assert(ob.getBuyOrders().size() == 1);
    assert(ob.getSellOrders().size() == 1);
}

void test_match_orders_empty_book() {
    OrderBook ob;
    ob.matchOrders(); // Should not crash
    assert(ob.getBuyOrders().empty());
    assert(ob.getSellOrders().empty());
}

int main() {
    test_add_and_cancel_order();
    test_match_orders_full_fill();
    test_match_orders_partial_fill();
    test_match_orders_no_match();
    test_match_orders_empty_book();
    std::cout << "OrderBook class tests passed!\n";
    return 0;
} 