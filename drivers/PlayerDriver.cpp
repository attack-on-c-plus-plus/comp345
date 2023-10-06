
#include <iostream>
#include "Drivers.h"
#include "../Player.h"

using namespace std;

void testplayer() {

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

    //---------------------------------

    // Get some territories from the loaded map
    Territory *territory1 = new Territory();
    Territory *territory2 = new Territory();
    Territory *territory3 = new Territory();

    // Associate territories with the player
    player->addTerritory(*territory1);
    player->addTerritory(*territory2);
    player->addTerritory(*territory3);

    // Print the player's territories
    cout << "Player " << player->getName() << " owns the following territories:\n";

    const std::vector<Territory>* playerTerritories = player->getTerritories();
    for (const Territory& territory : *playerTerritories) {
        std::cout << "Territory Name: " << territory.getName() << std::endl;
    }

};
