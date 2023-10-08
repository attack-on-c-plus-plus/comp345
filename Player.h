
#ifndef COMP345_PLAYER_H
#define COMP345_PLAYER_H

#include <string>
#include <vector>
#include "Cards.h"
#include "Map.h"
#include "Orders.h"

// Daniel Soldera
// Carson Senthilkumar
// Joe El-Khoury
// Henri Stephane Carbon
// Haris Mahmood

// forward declaration
class Map;

class Territory;

class OrdersList;

class Player {
public:
    //Constructor
    Player();

    explicit Player(const std::string &name);

    Player(const Player &other);

    //Destructor
    ~Player();

    //Methods
    [[nodiscard]] std::string &getName() const;
    void changeName(const std::string &newName);

    [[nodiscard]] std::vector<Territory> toAttack(const Map &) const;
    [[nodiscard]] std::vector<Territory> toDefend(const Map &) const;

    void issueOrder(const std::string &orderType, int sourceTerritory, int targetTerritory, int armies, int targetPlayerID);

    //method that views the order list of the player
    std::vector<Order*> getOrderList();

    // Add methods to manage the player's territory
    void addTerritory(Territory &territory);

    const std::vector<Territory>& getTerritories() const;

    // Add methods to manage the player's hand of cards
    void drawCardFromDeck(Deck &deck);

    [[nodiscard]] const Hand &getHand() const;

private:
    //Attributes
    std::string *name;
    std::vector<Territory> *territories;
    OrdersList *ordersList;
    Hand *hand;


};


#endif //COMP345_PLAYER_H
