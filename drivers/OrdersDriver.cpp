#include <iostream>
#include "Drivers.h"
#include "../Orders.h"

// Daniel Soldera
// Carson Senthilkumar
// Joe El-Khoury
// Henri Stephane Carbon
// Haris Mahmood

void testOrderList() {
    // Create some sample orders
    DeployOrder deployOrder(10);
    AdvanceOrder advanceOrder(1, 2, 5);
    BombOrder bombOrder(3);
    BlockadeOrder blockadeOrder(4);
    AirliftOrder airliftOrder(5, 6, 7);
    NegotiateOrder negotiateOrder(8);

    // Create an OrdersList and add orders to it
    OrdersList ordersList;
    ordersList.addOrder(&deployOrder)
            .addOrder(&advanceOrder)
            .addOrder(&bombOrder)
            .addOrder(&blockadeOrder)
            .addOrder(&airliftOrder)
            .addOrder(&negotiateOrder);

    // Execute the orders
    ordersList.executeOrders();

    // Display the results

    std::cout << "\n 1. Added all given orders to the Orderlist:\n";
    for (auto order : ordersList.getOrder()) {
        std::cout << *order << std::endl;
    }

    
    // remove method
    ordersList.remove(2);

    std::cout << "\n 2.Orders executed after removing a given order:\n";
    for (auto order : ordersList.getOrder()) {
        std::cout << *order << std::endl;
    }

}
