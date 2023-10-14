#include <iostream>
#include <string>
#include <vector>
#include "Player.h"

// Daniel Soldera
// Carson Senthilkumar
// Joe El-Khoury
// Henri Stephane Carbon
// Haris Mahmood

//Default Constructor
Player::Player() //: Player("Player Default")
        : name{new std::string("Test")}, ordersList{new OrdersList()}, territories{new std::vector<Territory>}, hand{new Hand()}
{
    std::cout << "* In the default constructor of player class * \n";
}

//Parameterized Constructor
Player::Player(const std::string &name)
        : name{new std::string(name)}, ordersList{new OrdersList()}, territories{new std::vector<Territory>},hand{new Hand()} {
    std::cout << "* In the Parameterized constructor of player class *\n";
}

//Copy Constructor
Player::Player(const Player &other)
        : name{new std::string(*(other.name))},
          ordersList{other.ordersList}, territories{new std::vector<Territory>(*other.territories)} {
    std::cout << "* In the copy constructor of player class *\n";
}

//Destructor
Player::~Player() {
   delete name;
   delete ordersList;
   delete hand;
   delete territories;
    std::cout << "Default Destructor is called\n";
}

std::string &Player::getName() const {
    return *name;
}

void Player::changeName(const std::string &newName) {
    name = new std::string(newName);
}


// Add a method to add a territory to the player's collection
void Player::addTerritory(Territory &territory) {
    territories->push_back(territory);
    territory.setOwner(*this);
}

const std::vector<Territory>& Player::getTerritories() const {
    return *territories;
}


std::vector<Territory> Player::toDefend(const Map &map) const {
    std::vector<Territory> territoriesToDefend;

    // Iterate through the player's territories
    for (const Territory &territory: *territories) {
        // Get the adjacent territories of the current territory
        std::vector<Territory> adjacentTerritories = map.getAdjacencies(territory);

        // Check if any adjacent territory is not owned by the player
        bool needsDefense = false;
        for (const Territory &adjacentTerritory: adjacentTerritories) {
            if (adjacentTerritory.getOwner().name != this->name) {
                needsDefense = true;
                break; // At least one adjacent territory is not owned by the player
            }
        }

        if (needsDefense) {
            // Add the current territory to the list of territories to defend
            territoriesToDefend.push_back(territory);
        }
    }

    return territoriesToDefend;
}


std::vector<Territory> Player::toAttack(const Map &map) const
{
    std::vector<Territory> territoriesToAttack;

    // Iterate through the player's territories
    for (const Territory &myTerritory: *territories) {
        // Get the adjacent territories of the current territory
        std::vector<Territory> adjacentTerritories = map.getAdjacencies(myTerritory);

        // Check if adjacent territories are owned by other players
        for (const Territory &adjacentTerritory: adjacentTerritories) {
            if (&(adjacentTerritory.getOwner()) != this) {

                // Check if adjacentTerritory is not already in territoriesToAttack
                bool isUnique = true;
                for (const Territory &attackTerritory : territoriesToAttack) {
                    if (attackTerritory.getName() == adjacentTerritory.getName()) {
                        isUnique = false;
                        break;
                    }
                }
                // If it's unique, add it to the list of territories to attack
                if (isUnique) {
                    territoriesToAttack.push_back(adjacentTerritory);
                }
            }
        }
    }

    return territoriesToAttack;
}


void Player::issueOrder(const std::string &orderType, int sourceTerritory, int targetTerritory, int armies,int targetPlayerID)
{
    if (orderType == "Deploy") {
        // Create a DeployOrder object
        DeployOrder d{armies};
        ordersList->addOrder(&d);
    } else if (orderType == "Advance") {
        // Create an AdvanceOrder object
        AdvanceOrder a{sourceTerritory, targetTerritory, armies};
        ordersList->addOrder(&a);
    } else if (orderType == "Bomb") {
        // Create a BombOrder object
        BombOrder b{targetTerritory};
        ordersList->addOrder(&b);
    } else if (orderType == "Blockade") {
        // Create a BlockadeOrder object
        BlockadeOrder b{targetTerritory};
        ordersList->addOrder(&b);
    } else if (orderType == "Airlift") {
        // Create an AirliftOrder object
        AirliftOrder a{armies, sourceTerritory, targetTerritory};
        ordersList->addOrder(&a);
    } else if (orderType == "Negotiate") {
        // Create a NegotiateOrder object with the target player ID
        NegotiateOrder n{targetPlayerID};
        ordersList->addOrder(&n);
    } else {
        return;
    }
}

void Player::drawCardFromDeck(Deck &deck) {
    deck.draw(*hand);
}

void Player::playCardFromHand(const Card &card, Deck &deck) {
    // Implement logic to play the card
    hand->discard(card.play(), deck);
}
const Hand &Player::getHand() const {
    return *hand;
}

std::vector<Order*> Player::getOrderList()
{
    return ordersList->getOrder();
}











