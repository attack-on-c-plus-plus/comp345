
#include <iostream>
#include "Drivers.h"
#include "../Player.h"

using namespace std;

// Daniel Soldera
// Carson Senthilkumar
// Joe El-Khoury
// Henri Stephane Carbon
// Haris Mahmood

void testplayer () {

    // Testing OrderList Functionalities
    //Object with pointer created on the heap (Manually have to delete)
    Player *player = new Player("Bob");

    std::cout<<"The Player Name is: "<<player->getName()<<"\n";

    // Issue some orders
    player->issueOrder("Deploy", 1, 2, 3, 4);
    player->issueOrder("Advance", 2, 3, 2, 5);
    player->issueOrder("Bomb", 3, 4, 0, 0);
    player->issueOrder("Blockade", 4, 5, 0, 0);
    player->issueOrder("Airlift", 5, 6, 4, 0);
    player->issueOrder("Negotiate", 7, 0, 0, 8);

    // Check the orders that were given
    std::cout << "Orders executed:\n";

    for (const Order* order : player->getOrderList())
    {
        std::cout << *order << std::endl;
    }

    // Testing Card Functionalities
    Player player1("Alice");

     // Create a deck and add some cards
    Deck deck;
    deck.addCard(Card("Power"));
    deck.addCard(Card("Reduction"));
    deck.addCard(Card("Card 3"));

    // Draw cards from the deck and add them to the player's hand
      player.drawCardFromDeck(deck);
      player.drawCardFromDeck(deck);

    // Get the player's hand
    Hand hand = player.getHand();

    // Verify the contents of the player's hand
    std::cout << "Player's hand contains the following cards:" << std::endl;
    for (unsigned int i = 0; i < hand.getCardCount(); i++)
    {
        std::cout << "Card: " << hand.getCard(i).getCardType() << std::endl;
    }
    
    //---------------------------------

     Player player2 ("Sally");
    Map map;
    MapLoader::load("maps/valid/3D.map", map);

    map.getTerritory(1).setOwner(player1);
   auto ts= map.getTerritories(player1);
};
