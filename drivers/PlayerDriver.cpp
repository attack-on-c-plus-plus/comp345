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

#include "PlayerDriver.h"

#include <cassert>
#include <iostream>

#include "../Cards.h"
#include "../CommandProcessing.h"
#include "../GameEngine.h"
#include "../Map.h"
#include "../Orders.h"
#include "../Player.h"

void createMap(Map &map);

void testPlayer() {
    const FakeRandom random;
    CommandProcessor commandProcessor;
    GameEngine gameEngine{commandProcessor, random};

    // Territories + Player Relation Testing : "Player owns a collection of territories (see Part 1)"
    const std::string seperator(70, '=');
    std::cout << seperator << std::endl;
    std::cout << "Displaying Territories + Player Relation Testing" << std::endl;
    std::cout << seperator << std::endl;

    const Player player1(gameEngine, "Sally", Strategy::Human);
    const Player player2(gameEngine, "Bob", Strategy::Human);

    auto &map = gameEngine.map();
    createMap(map);

    player1.add(map.territory(0));
    player1.add(map.territory(1));
    player2.add(map.territory(2));

    // Display the territories owned by the player
    std::cout << seperator << std::endl;
    std::cout << "Player Information:" << std::endl;
    std::cout << seperator << std::endl;

    std::cout << "Player \"" << player1.name() << "\" has the following Territories:" << std::endl;
    for (const auto& ownedTerritories = player1.territories(); const auto territory: ownedTerritories) {
        std::cout << "\tTerritory: " << *territory << std::endl;
    }

    // Cards + Player Relation Testing : "Player owns a hand of Warzone cards (see Part 5)"

    std::cout << seperator << std::endl;
    std::cout << "Displaying Cards + Player Relation Testing" << std::endl;
    std::cout << seperator << std::endl;

    // Create a deck and add some cards to it
    gameEngine.deck().add(CardType::bomb)
        .add(CardType::airlift)
        .add(CardType::blockade);

    // Draw cards from the deck and add them to the player's hand
    player1.draw();
    player1.draw();
    player1.draw();

    // Get the player's hand and display the cards
    const Hand& hand = player1.hand();
    std::cout << "\nPlayer's Hand:" << std::endl;
    for (unsigned int i = 0; i < hand.size(); i++) {
        std::cout << "Card " << i + 1 << ": " << hand.card(i) << std::endl;
    }
    std::cout << "\n" << std::endl;

    // Territories + Player Relation Testing : "Player contains methods toDefend() and toAttack() that return a list of territories that are to be defended and to be attacked, respectively. "

    std::cout << seperator << std::endl;
    std::cout << "Displaying Territories + Player Relation Testing" << std::endl;
    std::cout << seperator << std::endl;

    std::cout << "Map Information:" << std::endl;

    // Display the territories owned by the players
    std::cout << seperator << std::endl;
    std::cout << "Player Information:" << std::endl;
    std::cout << seperator << std::endl;

    std::cout << "Player " << player1.name() << "\" Has the following Territories:"
              << std::endl;
    for (const auto& ownedTerritories2 = player1.territories(); const auto territory: ownedTerritories2) {
        std::cout << "\tTerritory: " << *territory << std::endl;
    }

    // Display the territories owned by the players
    std::cout << seperator << std::endl;
    std::cout << "Player Information:" << std::endl;
    std::cout << seperator << std::endl;
    std::cout << "Player " << player2.name() << "\" Has the following Territories:"
              << std::endl;
    for (const auto& ownedTerritories3 = player2.territories(); const auto territory: ownedTerritories3) {
        std::cout << "\tTerritory: " << *territory << std::endl;
    }

    const std::vector<const Territory *> attackTerritories = player1.toAttack();

    // Display the territories to attack
    std::cout << "Player " << player1.name() << " (attacking) territories:" << std::endl;
    for (const auto territory: attackTerritories) {
        std::cout << *territory << std::endl;
    }

    const std::vector<const Territory *> defendTerritories = player2.toDefend();

    // Display the territories to defend
    std::cout << "Player " << player2.name() << " (defending) territories:" << std::endl;
    for (const auto territory: defendTerritories) {
        std::cout << *territory << std::endl;
    }

//------------------------------------------------------------------------------------------------------

    // OrderList + Player Relation Testing : "Player contains a issueOrders() method that creates an order object and adds it to the list of orders."
    // Display the territories owned by the players
    std::cout << seperator << std::endl;
    std::cout << "Player Information:" << std::endl;
    std::cout << seperator << std::endl;
    std::cout << "Displaying OrderList + Player Relation" << std::endl;
    std::cout << seperator << std::endl;

    // Issue some orders
    player1.issueOrders();

    // Check the orders that were given
    std::cout << seperator << std::endl;
    std::cout << "Orders executed: Player Name is " << player1.name() << std::endl;
    std::cout << seperator << std::endl;

    std::cout << player1.orderList() << std::endl;
//-----------------------------------------------------------------------------------------------------------
}

void testPlayers() {
    CommandProcessor commandProcessor{};
    const FakeRandom fakeRandom;
    GameEngine gameEngine{commandProcessor, fakeRandom};
    const Players players{gameEngine};

    const Player p1{gameEngine, "Bob", Strategy::Human};
    const Player p2{gameEngine, "Bob", Strategy::Neutral};
    const Player p3{gameEngine, "Bob", Strategy::Aggressive};
    const Player p4{gameEngine, "Jane", Strategy::Aggressive};

    std::cout << std::boolalpha;
    std::cout << players.add(p1) << std::endl;
    std::cout << players.add(p2) << std::endl;
    std::cout << players.add(p3) << std::endl;
    std::cout << players.add(p4) << std::endl;
}


void createMap(Map &map) {
    const Continent c1{"c1", 1};
    const Continent c2{"c2", 2};
    map.add(c1).add(c2);
    const Territory t1{"t1"}, t2{"t2"}, t3{"t3"};
    map.add(t1).add(t2).add(t3);
    map.addEdge(t1, t2).addEdge(t1, t3);
    map.addEdge(t2, t1);
    map.addEdge(t3, t1);
    map.addEdge(c1,t1);
    map.addEdge(c2,t2).addEdge(c2,t3);
}
