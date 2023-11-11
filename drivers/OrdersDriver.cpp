/**
 ************************************
 * COMP 345 Professor Hakim Mellah
 ************************************
 * @author Team 5 Attack on C++
 * @author Daniel Soldera
 * @author Carson Senthilkumar
 * @author Joe El-Khoury
 * @author Henri Stephane Carbon
 * @author Haris Mahmood
 */

#include <iostream>

#include "Drivers.h"
#include "../CommandProcessing.h"
#include "../GameEngine.h"
#include "../Map.h"
#include "../Orders.h"
#include "../Player.h"

void createMap(Map &map);

void testOrderList() {
    CommandProcessor commandProcessor;
    GameEngine engine{commandProcessor};

    Player player1{engine, "player1"};
    Player player2{engine, "player2"};

    auto &map = engine.map();
    createMap(map);

    player1.add(map.territory(0));
    player1.add(map.territory(1));
    player2.add(map.territory(2));

    // Create some sample orders
    auto *deployOrder = new DeployOrder(engine, player1, map.territory(0), 10);
    auto *advanceOrder = new AdvanceOrder(engine, player1, map.territory(0), map.territory(1), 5);
    auto *bombOrder = new BombOrder(engine, player1, map.territory(2));
    auto *blockadeOrder = new BlockadeOrder(engine, player1, map.territory(1));
    auto *airliftOrder = new AirliftOrder(engine, player1, map.territory(1), map.territory(0), 2);
    auto *negotiateOrder = new NegotiateOrder(engine, player1, player2);

    // Create an OrdersList and add orders to it
    OrdersList ordersList;
    ordersList.addOrder(*deployOrder)
            .addOrder(*advanceOrder)
            .addOrder(*bombOrder)
            .addOrder(*blockadeOrder)
            .addOrder(*airliftOrder)
            .addOrder(*negotiateOrder);

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
}
