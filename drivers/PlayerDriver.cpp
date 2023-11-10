
#include <iostream>
#include "Drivers.h"
#include "../Player.h"

using namespace std;

// Daniel Soldera
// Carson Senthilkumar
// Joe El-Khoury
// Henri Stephane Carbon
// Haris Mahmood

void createMap(Map &map);

void testPlayer() {

//------------------------------------------------------------------------------------------------------

    CommandProcessor commandProcessor;
    GameEngine gameEngine{commandProcessor};

    // Territories + Player Relation Testing : "Player owns a collection of territories (see Part 1)"

    std::cout << "\nDisplaying Territories + Player Relation Testing\n";
    std::cout << "********************************************\n";
    Player player1(gameEngine, "Sally");
    Player player2(gameEngine, "Bob");

    auto &map = gameEngine.map();
    createMap(map);

    player1.add(map.territory(0));
    player1.add(map.territory(1));
    player2.add(map.territory(2));

    // Display the territories owned by the player
    std::cout << "\nPlayer Information:\nThe Player \"" << player1.getName() << "\" Has the following Territories:"
              << std::endl;
    auto ownedTerritories = player1.getTerritories();
    for (auto territory: ownedTerritories) {
        std::cout << "\tTerritory: " << *territory << std::endl;
    }

//------------------------------------------------------------------------------------------------------

    // Cards + Player Relation Testing : "Player owns a hand of Warzone cards (see Part 5)"

    std::cout << "\nDisplaying Cards + Player Relation Testing\n";
    std::cout << "********************************************\n";

    // Create a deck and add some cards to it
    gameEngine.getDeck().add(CardType::bomb)
        .add(CardType::airlift)
        .add(CardType::blockade);

    // Draw cards from the deck and add them to the player's hand
    player1.draw();
    player1.draw();
    player1.draw();

    // Get the player's hand and display the cards
    Hand hand = player1.getHand();
    std::cout << "\nPlayer's Hand:" << std::endl;
    for (unsigned int i = 0; i < hand.size(); i++) {
        std::cout << "Card " << i + 1 << ": " << hand.card(i) << std::endl;
    }
    std::cout << "\n" << std::endl;

//------------------------------------------------------------------------------------------------------
    // Territories + Player Relation Testing : "Player contains methods toDefend() and toAttack() that return a list of territories that are to be defended and to be attacked, respectively. "

    std::cout << "\nDisplaying Territories + Player Relation Testing\n";
    std::cout << "********************************************\n";
    std::cout << "\nMap Information:\n";

    // Display the territories owned by the players
    std::cout << "\nPlayer Information:\nPlayer " << player1.getName() << "\" Has the following Territories:"
              << std::endl;
    auto ownedTerritories2 = player1.getTerritories();
    for (auto territory: ownedTerritories2) {
        std::cout << "\tTerritory: " << *territory << std::endl;
    }

    std::cout << "\nPlayer Information:\nPlayer " << player2.getName() << "\" Has the following Territories:"
              << std::endl;
    auto ownedTerritories3 = player2.getTerritories();
    for (auto territory: ownedTerritories3) {
        std::cout << "\tTerritory: " << *territory << std::endl;
    }

    std::vector<const Territory *> attackTerritories = player1.toAttack();

    // Display the territories to attack
    std::cout << "\nPlayer " << player1.getName() << " (attacking) territories:" << std::endl;
    for (auto territory: attackTerritories) {
        std::cout << *territory << std::endl;
    }

    std::vector<const Territory *> defendTerritories = player2.toDefend();

    // Display the territories to defend
    std::cout << "\nPlayer " << player2.getName() << " (defending) territories:" << std::endl;
    for (auto territory: defendTerritories) {
        std::cout << *territory << std::endl;
    }

//------------------------------------------------------------------------------------------------------

    // OrderList + Player Relation Testing : "Player contains a issueOrder() method that creates an order object and adds it to the list of orders."
    std::cout << "\nDisplaying OrderList + Player Relation\n";
    std::cout << "**************************************\n";

    // Issue some orders
    player1.issueOrder();
    player1.issueOrder();
    player1.issueOrder();
    player1.issueOrder();
    player1.issueOrder();
    player1.issueOrder();

    // Check the orders that were given
    std::cout << "\n\tOrders executed: Player Name is " << player1.getName() << std::endl;
    std::cout << "****************" << std::endl;

    std::cout << player1.orderList() << std::endl;
//-----------------------------------------------------------------------------------------------------------
};

void createMap(Map &map) {
    Continent c1{"c1", 1}, c2{"c2", 2};
    map.add(c1).add(c2);
    Territory t1{"t1"}, t2{"t2"}, t3{"t3"};
    map.add(t1).add(t2).add(t3);
    map.addEdge(t1, t2).addEdge(t1, t3);
    map.addEdge(t2, t1);
    map.addEdge(t3, t1);
    map.addEdge(c1,t1);
    map.addEdge(c2,t2).addEdge(c2,t3);
}
