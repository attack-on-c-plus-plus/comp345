#include <iostream>
#include "Drivers.h"
#include "../Orders.h"

// Daniel Soldera
// Carson Senthilkumar
// Joe El-Khoury
// Henri Stephane Carbon
// Haris Mahmood

void testOrderList() {
    CommandProcessor *commandProcessor = new CommandProcessor();
    GameEngine engine{GameState::start, *commandProcessor};

    Player p1{"p1"};
    Player p2{"p2"};

    Territory t1{};
    Territory t2{};
    Territory t3{};

    t1.owner(p1);
    t2.owner(p1);
    t3.owner(p2);

    // Create some sample orders
    DeployOrder deployOrder{p1, t1, 10, engine};
    AdvanceOrder advanceOrder{p1, t1, t2, 5, engine};
    BombOrder bombOrder{p1, t3, engine};
    BlockadeOrder blockadeOrder{p1, t2, engine};
    AirliftOrder airliftOrder{p1, t2, t1, 2, engine};
    NegotiateOrder negotiateOrder{p2, p1, engine};

    // Create an OrdersList and add orders to it
    OrdersList ordersList;
    ordersList.addOrder(deployOrder)
            .addOrder(advanceOrder)
            .addOrder(bombOrder)
            .addOrder(blockadeOrder)
            .addOrder(airliftOrder)
            .addOrder(negotiateOrder);

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

    // move method
    ordersList.move(2,3);
    std::cout << "\n 3.Orders executed after moving a given order:\n";
    for (auto order : ordersList.getOrder()) {
        std::cout << *order << std::endl;
    }
    delete commandProcessor;
}
