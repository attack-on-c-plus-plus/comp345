
#include <iostream>
#include "Drivers.h"
#include "../Player.h"

using namespace std;

// Daniel Soldera
// Carson Senthilkumar
// Joe El-Khoury
// Henri Stephane Carbon
// Haris Mahmood

void testPlayer() {

//------------------------------------------------------------------------------------------------------

    // Territories + Player Relation Testing : "Player owns a collection of territories (see Part 1)"

    std::cout << "\nDisplaying Territories + Player Relation Testing\n";
    std::cout << "********************************************\n";
    Player player("Sally");
    Map map;
    std::cout << "\nMap Information:\n";
    MapLoader::load("maps/valid/3D.map", map);

    Territory &territoryToAdd1 = map.getTerritory(3);
    Territory &territoryToAdd2 = map.getTerritory(4);

    player.addTerritory(territoryToAdd1);
    player.addTerritory(territoryToAdd2);

    // Display the territories owned by the player
    std::cout << "\nPlayer Information:\nThe Player \"" << player.getName() << "\" Has the following Territories:"
              << std::endl;
    const std::vector<Territory> &ownedTerritories = player.getTerritories();
    for (const Territory &territory: ownedTerritories) {
        std::cout << "\tTerritory ID: " << territory.getId() << ", Name: " << territory.getName() << std::endl;
    }

//------------------------------------------------------------------------------------------------------

    // Cards + Player Relation Testing : "Player owns a hand of Warzone cards (see Part 5)"

    std::cout << "\nDisplaying Cards + Player Relation Testing\n";
    std::cout << "********************************************\n";

    // Create a deck and add some cards to it
    Deck deck;
    deck.add(new Card(CardType::bomb));
    deck.add(new Card(CardType::airlift));
    deck.add(new Card(CardType::blockade));

    // Create a player
    Player player1("Player1");

    // Draw cards from the deck and add them to the player's hand
    player1.drawCardFromDeck(deck);
    player1.drawCardFromDeck(deck);
    player1.drawCardFromDeck(deck);

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
    Player playerA("Sally");
    Player playerB("Richard");
    Map map1;
    std::cout << "\nMap Information:\n";
    MapLoader::load("maps/valid/3D.map", map1);

    Territory &territoryToAdd3 = map1.getTerritory(3);
    Territory &territoryToAdd4 = map1.getTerritory(4);
    Territory &territoryToAdd5 = map1.getTerritory(2);

    playerA.addTerritory(territoryToAdd3);
    playerA.addTerritory(territoryToAdd4);
    playerB.addTerritory(territoryToAdd5);

    // Display the territories owned by the players
    std::cout << "\nPlayer Information:\nPlayer " << playerA.getName() << "\" Has the following Territories:"
              << std::endl;
    const std::vector<Territory> &ownedTerritories2 = playerA.getTerritories();
    for (const Territory &territory: ownedTerritories2) {
        std::cout << "\tTerritory ID: " << territory.getId() << ", Name: " << territory.getName() << std::endl;
    }

    std::cout << "\nPlayer Information:\nPlayer " << playerB.getName() << "\" Has the following Territories:"
              << std::endl;
    const std::vector<Territory> &ownedTerritories3 = playerB.getTerritories();
    for (const Territory &territory: ownedTerritories3) {
        std::cout << "\tTerritory ID: " << territory.getId() << ", Name: " << territory.getName() << std::endl;
    }

    std::vector<Territory> attackTerritories = playerA.toAttack(map1);

    // Display the territories to attack
    std::cout << "\nPlayer " << playerA.getName() << " (attacking) territories:" << std::endl;
    for (const Territory &territory: attackTerritories) {
        std::cout << territory.getName() << std::endl;
    }

    std::vector<Territory> defendTerritories = playerB.toDefend(map1);

    // Display the territories to defend
    std::cout << "\nPlayer " << playerB.getName() << " (defending) territories:" << std::endl;
    for (const Territory &territory: defendTerritories) {
        std::cout << territory.getName() << std::endl;
    }

//------------------------------------------------------------------------------------------------------

    // OrderList + Player Relation Testing : "Player contains a issueOrder() method that creates an order object and adds it to the list of orders."
    std::cout << "\nDisplaying OrderList + Player Relation\n";
    std::cout << "**************************************\n";
    //Object with pointer created on the heap (Manually have to delete)
    Player playerX{"Bob"};

    // Issue some orders
    playerX.issueOrder("Deploy", 1, 2, 3, 4);
    playerX.issueOrder("Advance", 2, 3, 2, 5);
    playerX.issueOrder("Bomb", 3, 4, 0, 0);
    playerX.issueOrder("Blockade", 4, 5, 0, 0);
    playerX.issueOrder("Airlift", 5, 6, 4, 0);
    playerX.issueOrder("Negotiate", 7, 0, 0, 8);

    // Check the orders that were given
    std::cout << "\n\tOrders executed: Player Name is " << playerX.getName() << "\n";
    std::cout << "\t****************\n";

    for (const Order *order: playerX.getOrderList()) {
        std::cout << "\t" << *order << std::endl;
    }

//-----------------------------------------------------------------------------------------------------------
};
