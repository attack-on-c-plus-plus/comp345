
#include <iostream>
#include "Drivers.h"
#include "../Player.h"

using namespace std;

// Daniel Soldera
// Carson Senthilkumar
// Joe El-Khoury
// Henri Stephane Carbon
// Haris Mahmood

void testPlayer () {
    //Object with pointer created on the heap (Manually have to delete)
    Player player{"Bob"};

    std::cout<<"The Player Name is: "<<player.getName()<<"\n";

    // Issue some orders
    player.issueOrder("Deploy", 1, 2, 3, 4);
    player.issueOrder("Advance", 2, 3, 2, 5);
    player.issueOrder("Bomb", 3, 4, 0, 0);
    player.issueOrder("Blockade", 4, 5, 0, 0);
    player.issueOrder("Airlift", 5, 6, 4, 0);
    player.issueOrder("Negotiate", 7, 0, 0, 8);

    // Check the orders that were given
    std::cout << "Orders executed:\n";

    for (const Order* order : player.getOrderList())
    {
      std::cout << *order << std::endl;
    }

    //---------------------------------

    Player player1 ("Sally");
    Map map;
    MapLoader::load("maps/valid/3D.map", map);

    map.getTerritory(1).setOwner(player1);
    auto ts= map.getTerritories(player1);
};
