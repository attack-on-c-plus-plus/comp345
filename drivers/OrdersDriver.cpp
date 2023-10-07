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

    std::cout << "Orders executed:\n";
    for (auto order : ordersList.getOrder()) {
        std::cout << *order << std::endl;
    }

    // TODO: Memory leak here
    //ordersList.remove(2);

    std::cout << "\n 2.Orders executed:\n";
    for (auto order : ordersList.getOrder()) {
        std::cout << *order << std::endl;
    }

    ordersList.move(3,6);
    std::cout << "\n 3.Orders executed:\n";
    for (auto order : ordersList.getOrder()) {
        std::cout << *order << std::endl;
    }
}