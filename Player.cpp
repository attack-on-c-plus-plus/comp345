#include <iostream>
#include <string>
#include <vector>
#include "Player.h"

// Daniel Soldera
// Carson Senthilkumar
// Joe El-Khoury
// Henri Stephane Carbon
// Haris Mahmood

//Parameterized Constructor
Player::Player(const std::string &name)
    : name{new std::string(name)}, ordersList{new OrdersList()}, territories{new std::vector<Territory *>},
    hand{new Hand()}, reinforcementPool{new int(50)}, cantTarget{new std::vector<Player *>} {}

//Copy Constructor
Player::Player(const Player &other)
    : name{new std::string(*(other.name))},
      ordersList{other.ordersList}, territories{new std::vector<Territory *>(*other.territories)},
      reinforcementPool{new int(*other.reinforcementPool)}, cantTarget{new std::vector<Player *>(*other.cantTarget)} {}

//Destructor
Player::~Player() {
   delete name;
   delete ordersList;
   delete hand;
   delete territories;
   delete reinforcementPool;
   delete cantTarget;
}

std::string &Player::getName() const {
    return *name;
}

void Player::changeName(const std::string &newName) {
    name = new std::string(newName);
}

/**
 * Gives the number of armies in the reinforcement pool
 */
int Player::getReinforcmentNumber() const
{
    return *reinforcementPool;
}
/**
 * Adds reinforcements to the pool
 */
void Player::addReinforcements(int armies)
{
    *reinforcementPool = *reinforcementPool + armies;
}

// Add a method to add a territory to the player's collection
void Player::addTerritory(Territory &territory) {
    territories->push_back(&territory);
    territory.owner(*this);
}

const std::vector<Territory *> &Player::getTerritories() const {
    return *territories;
}

const std::vector<Player *> &Player::getCantAttack() const
{
    return *cantTarget;
}

void Player::removeNegotiators()
{
    cantTarget->clear();
}

void Player::addNegotiator(Player negotiator) const
{
    cantTarget->push_back(&negotiator);
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

                // Check that owner is not on cantAttack list because of negotiate order
                bool canAttack = true;
                for (auto player : *cantTarget)
                {
                    if (&(adjacentTerritory->owner()) == &(*player))
                    {
                        canAttack = false;
                        break;
                    }
                }

                // Check if adjacentTerritory is not already in territoriesToAttack
                bool isUnique = true;
                for (auto attackTerritory : territoriesToAttack) {
                    if (attackTerritory->name() == adjacentTerritory->name()) {
                        isUnique = false;
                        break;
                    }
                }
                // If it's unique, add it to the list of territories to attack
                if (isUnique && canAttack)
                {
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

OrdersList &Player::orderList()
{
    return *ordersList;
}











