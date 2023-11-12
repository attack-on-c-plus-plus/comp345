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

#include "OrdersDriver.h"

#include <iostream>
missing tests
#include "../CommandProcessing.h"
#include "../GameEngine.h"
#include "../Map.h"
#include "../Orders.h"
#include "../Player.h"
#include "../Cards.h"

void createMap(Map &map);

void testOrderList()
{
    const std::string seperator(70, '=');
    std::cout << seperator << std::endl;
    std::cout << "Testing Order List" << std::endl;
    std::cout << seperator << std::endl;

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
            .addOrder(*blockadeOrder)missing tests
            .addOrder(*airliftOrder)
            .addOrder(*negotiateOrder);

    // Execute the orders
    ordersList.executeOrders();

    // Display the results

    std::cout << seperator << std::endl;
    std::cout << " 1. Added all given orders to the Orderlist:" << std::endl;
    std::cout << seperator << std::endl;
    for (const auto order : ordersList.getOrder()) {
        std::cout << *order << std::endl;
    }

    // remove method
    ordersList.remove(2);

    std::cout << seperator << std::endl;
    std::cout << " 2.Orders executed after removing a given order:" << std::endl;
    std::cout << seperator << std::endl;
    for (const auto order : ordersList.getOrder()) {
        std::cout << *order << std::endl;
    }

    // move method
    ordersList.move(2,3);
    std::cout << seperator << std::endl;
    std::cout << " 3.Orders executed after moving a given order:" << std::endl;
    std::cout << seperator << std::endl;
    for (const auto order : ordersList.getOrder()) {
        std::cout << *order << std::endl;
    }
}

void testOrderExecution()
{
    const std::string seperator(70, '=');
    std::cout << seperator << std::endl;
    std::cout << "Testing Order Execution" << std::endl;
    std::cout << seperator << std::endl;

    // Create a Map, Players, and Territories for testing

    CommandProcessor commandProcessor;
    GameEngine engine{commandProcessor};

    Player player1{engine, "player1"};
    Player player2{engine, "player2"};

    engine.getPlayers().push_back(&player1);
    engine.getPlayers().push_back(&player2);

    auto &map = engine.map();
    createMap(map);

    player1.add(map.territory(0));
    player1.add(map.territory(1));
    player2.add(map.territory(2));


    //Original territory information before any execution
    std::cout << seperator << std::endl;
    std::cout << "Original Territory Information:" << std::endl;
    std::cout << seperator << std::endl;
    for (const auto t : engine.map().territories()) {
        std::cout << *t << " owned by " << t->owner() << " has " << t->armyCount() << " armies" << std::endl;
    }

    //Created a deploy order to showcase its capability.
    auto *deployOrder = new DeployOrder(engine, player1, map.territory(0), 10);
    auto *deployOrder2 = new DeployOrder(engine, player1, map.territory(1), 10);
    auto *deployOrder3 = new DeployOrder(engine, player2, map.territory(2), 10);
    auto *deployOrderInvalid = new DeployOrder(engine, player2, map.territory(0), 10);

    //Created a list
    OrdersList ordersList;

    //Added the deploy order to the list
    ordersList.addOrder(*deployOrder)
              .addOrder(*deployOrder2)
              .addOrder(*deployOrder3)
              .addOrder(*deployOrderInvalid);

    //Executed the order
    ordersList.executeOrders();

    //------------------------------------------------------------------------------------------------------------------
    //                                               Deployment
    //------------------------------------------------------------------------------------------------------------------
    std::cout << seperator << std::endl;
    std::cout << "1.Deployment Order:" << std::endl;
    std::cout << seperator << std::endl;
    for (const auto order : ordersList.getOrder()) {
        std::cout << *order << std::endl;
    }

    //Removed the deploy order from the list
    ordersList.remove(0);
    ordersList.remove(0);
    ordersList.remove(0);
    ordersList.remove(0);


    //Information of territory,army and owner after deploy execution
    std::cout << seperator << std::endl;
    std::cout << "Result: " << std::endl;
    std::cout << seperator << std::endl;
    for (const auto t : engine.map().territories()) {
        std::cout << *t << " owned by " << t->owner() << " has " << t->armyCount() << " armies" << std::endl;
    }

    //------------------------------------------------------------------------------------------------------------------
    //                                               Advance + Cards
    //------------------------------------------------------------------------------------------------------------------
    std::cout << seperator << std::endl;
    std::cout << "2 & 3.Advance Order: " << std::endl;
    std::cout << seperator << std::endl;

    //Case where both source and target territory belong to a player
    auto *advanceOrder = new AdvanceOrder(engine, player1, map.territory(0), map.territory(1), 5);

    ordersList.addOrder(*advanceOrder);

    ordersList.executeOrders();

    for (const auto order : ordersList.getOrder())
    {
        std::cout << *order << std::endl;
    }

    //Information of territory,army and owner after advance execution
    std::cout << seperator << std::endl;
    std::cout <<"Result:" << std::endl;
    std::cout << seperator << std::endl;
    for (const auto t : engine.map().territories())
    {
        std::cout << *t << " owned by " << t->owner() << " has " << t->armyCount() << " armies" << std::endl;
    }

    ordersList.remove(0);

    std::cout << seperator << std::endl;
    std::cout<<"* Case where attacker attacks defender territory (with less troops) *" << std::endl;
    std::cout << seperator << std::endl;
    //Case where attacker attacks defender territory (with less troops)
    auto *advanceOrder1 = new AdvanceOrder(engine, player1, map.territory(0), map.territory(2), 5);

    ordersList.addOrder(*advanceOrder1);

    for (const auto order : ordersList.getOrder())
    {
        std::cout << *order << std::endl;
    }

    ordersList.executeOrders();
    ordersList.remove(0);

    //Information of territory,army and owner after advance execution
    std::cout << seperator << std::endl;
    std::cout <<"Result:" << std::endl;
    std::cout << seperator << std::endl;
    for (const auto t : engine.map().territories())
    {
        std::cout << *t << " owned by " << t->owner() << " has " << t->armyCount() << " armies" << std::endl;
    }

    std::cout << seperator << std::endl;
    std::cout <<"Players 1 Hand:" << std::endl;
    std::cout << seperator << std::endl;
    for (const auto card : player1.getHand().cards()) { std::cout << *card << std::endl; }

    std::cout << seperator << std::endl;
    std::cout<<"* Case where attacker attacks defender territory (with more troops) *" << std::endl;
    std::cout << seperator << std::endl;
    //Case where attacker attacks defender territory (with more troops)
    auto *advanceOrder2 = new AdvanceOrder(engine, player1, map.territory(1), map.territory(2), 15);

    ordersList.addOrder(*advanceOrder2);

    for (const auto order : ordersList.getOrder())
    {
        std::cout << *order << std::endl;
    }

    ordersList.executeOrders();

    //Information of territory,army and owner after advance execution
    std::cout << seperator << std::endl;
    std::cout <<"Result:" << std::endl;
    std::cout << seperator << std::endl;
    for (const auto t : engine.map().territories())
    {
        std::cout << *t << " owned by " << t->owner() << " has " << t->armyCount() << " armies" << std::endl;
    }

    ordersList.remove(0);

    std::cout <<"Players 1 Hand:" << std::endl;
    for (const auto card : player1.getHand().cards()) { std::cout << *card << std::endl; }

    //------------------------------------------------------------------------------------------------------------------
    //                                               Negotiate
    //------------------------------------------------------------------------------------------------------------------
    std::cout << seperator << std::endl;
    std::cout <<"4.Negotiate Order:" << std::endl;
    std::cout << seperator << std::endl;

    //Default giving player 2 territory 3
    player2.add(map.territory(2));
    auto *deployOrder4 = new DeployOrder(engine, player2, map.territory(2), 10);
    auto *deployOrder5 = new DeployOrder(engine, player1, map.territory(0), 10);
    ordersList.addOrder(*deployOrder4)
              .addOrder(*deployOrder5);
    ordersList.executeOrders();
    ordersList.remove(0);
    ordersList.remove(0);


    std::cout << seperator << std::endl;
    std::cout <<"Originally:" << std::endl;
    std::cout << seperator << std::endl;
    for (const auto t : engine.map().territories())
    {
        std::cout << *t << " owned by " << t->owner() << " has " << t->armyCount() << " armies" << std::endl;
    }

    auto *negotiateOrder = new NegotiateOrder(engine, player1, player2);
    ordersList.addOrder(*negotiateOrder);
    ordersList.executeOrders();

    std::cout <<"" << std::endl;
    for (const auto order : ordersList.getOrder())
    {
        std::cout << *order << std::endl;
    }

    ordersList.remove(0);
    std::cout << seperator << std::endl;
    std::cout <<"If Player 2 Attempts to Attack Player 1:" << std::endl;
    std::cout << seperator << std::endl;
    auto *advanceOrder3 = new AdvanceOrder(engine, player2, map.territory(2), map.territory(0), 10);
    ordersList.addOrder(*advanceOrder3);
    ordersList.executeOrders();
    std::cout <<"\n";
    for (auto order : ordersList.getOrder())
    {
        std::cout << *order << std::endl;
    }

     ordersList.remove(0);
    std::cout <<"\nResults:\n";
    for (auto t : engine.map().territories())
    {
        std::cout << *t << " owned by " << t->owner() << " has " << t->armyCount() << " armies" << std::endl;
    }

    //------------------------------------------------------------------------------------------------------------------
    //                                               Blockade
    //------------------------------------------------------------------------------------------------------------------
    std::cout << seperator << std::endl;
    std::cout << "5.Blockade Order:" << std::endl;
    std::cout << seperator << std::endl;
    auto *blockadeOrder = new BlockadeOrder(engine, player1, map.territory(0));
    std::cout <<"Originally:" << std::endl;
    for (const auto t : engine.map().territories())
    {
        std::cout << *t << " owned by " << t->owner() << " has " << t->armyCount() << " armies" << std::endl;
    }

    ordersList.addOrder(*blockadeOrder);
    ordersList.executeOrders();
    std::cout << seperator << std::endl;
    std::cout <<"" << std::endl;
    std::cout << seperator << std::endl;
    for (const auto order : ordersList.getOrder())
    {
        std::cout << *order << std::endl;
    }

    ordersList.remove(0);

    std::cout << seperator << std::endl;
    std::cout <<"After:" << std::endl;
    std::cout << seperator << std::endl;
    for (const auto t : engine.map().territories())
    {
        std::cout << *t << " owned by " << t->owner() << " has " << t->armyCount() << " armies" << std::endl;
    }

    engine.getPlayers().clear();
}

