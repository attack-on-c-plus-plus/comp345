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
        : name{new std::string("Test")}, ordersList{new OrdersList()}, territories{new std::vector<Territory *>}, hand{new Hand()}
{
    std::cout << "* In the default constructor of player class * \n";
}

//Parameterized Constructor
Player::Player(const std::string &name)
        : name{new std::string(name)}, ordersList{new OrdersList()}, territories{new std::vector<Territory *>},hand{new Hand()} {
    std::cout << "* In the Parameterized constructor of player class *\n";
}

//Copy Constructor
Player::Player(const Player &other)
        : name{new std::string(*(other.name))},
          ordersList{other.ordersList}, territories{new std::vector<Territory *>(*other.territories)} {
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
    territories->push_back(&territory);
    territory.owner(*this);
}

const std::vector<Territory *> &Player::getTerritories() const {
    return *territories;
}


std::vector<const Territory *> Player::toDefend(const Map &map) const {
    std::vector<const Territory *> territoriesToDefend;

    // Iterate through the player's territories
    for (auto territory: *territories) {
        // Get the adjacent territories of the current territory
        auto adjacentTerritories = map.adjacencies(*territory);

        // Check if any adjacent territory is not owned by the player
        bool needsDefense = false;
        for (auto adjacentTerritory: adjacentTerritories) {
            if (adjacentTerritory->owner().name != this->name) {
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


std::vector<const Territory *> Player::toAttack(const Map &map) const
{
    std::vector<const Territory*> territoriesToAttack;

    // Iterate through the player's territories
    for (auto myTerritory: *territories) {
        // Get the adjacent territories of the current territory
        auto adjacentTerritories = map.adjacencies(*myTerritory);

        // Check if adjacent territories are owned by other players
        for (auto adjacentTerritory: adjacentTerritories) {
            if (&(adjacentTerritory->owner()) != this) {

                // Check if adjacentTerritory is not already in territoriesToAttack
                bool isUnique = true;
                for (auto attackTerritory : territoriesToAttack) {
                    if (attackTerritory->name() == adjacentTerritory->name()) {
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


void Player::issueOrder(const std::string &orderType, Territory &source, Territory &target, unsigned armies, const Player &otherPlayer, GameEngine &gameEngine)
{
    if (orderType == "Deploy") {
        // Create a DeployOrder object
        ordersList->addOrder(DeployOrder{*this, target, armies, gameEngine});
    } else if (orderType == "Advance") {
        // Create an AdvanceOrder object
        ordersList->addOrder(AdvanceOrder{*this, source, target, armies, gameEngine});
    } else if (orderType == "Bomb") {
        // Create a BombOrder object
        ordersList->addOrder(BombOrder{*this, target, gameEngine});
    } else if (orderType == "Blockade") {
        // Create a BlockadeOrder object
        ordersList->addOrder(BlockadeOrder{*this, target, gameEngine});
    } else if (orderType == "Airlift") {
        // Create an AirliftOrder object
        ordersList->addOrder(AirliftOrder{*this, source, target, armies, gameEngine});
    } else if (orderType == "Negotiate") {
        // Create a NegotiateOrder object with the target player ID
        ordersList->addOrder(NegotiateOrder{*this, otherPlayer, gameEngine});
    } else {
        return;
    }
}

void Player::drawCardFromDeck(Deck &deck) {
    deck.draw(*hand);
}

void Player::playCardFromHand(const Card &card, Deck &deck, GameEngine &engine, Territory &target) {
    // Implement logic to play the card
    // Right now, I'll create a custom territory class
    deck.discard(card.play(*this, target, engine), *hand);
}
const Hand &Player::getHand() const {
    return *hand;
}

std::vector<Order*> Player::getOrderList()
{
    return ordersList->getOrder();
}











