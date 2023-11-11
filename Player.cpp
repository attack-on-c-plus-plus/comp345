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

#include "Player.h"

#include <algorithm>
#include <string>
#include <vector>

#include "Cards.h"
#include "GameEngine.h"
#include "Map.h"
#include "Orders.h"

/**
 * Default Constructor
 */
Player::Player(GameEngine &gameEngine, const std::string &name) :
    name{new std::string(name)}, ordersList{new OrdersList()}, territories{new std::vector<Territory *>},
    hand{new Hand()}, reinforcementPool{new unsigned(50)}, cantTarget{new std::vector<Player *>}
{
    gameEngine_ = &gameEngine;
    commandProcessor_ = nullptr;
}

/**
 * Parameterized Constructor
 */
Player::Player(GameEngine &gameEngine, const std::string &name, CommandProcessor &commandProcessor) :
    Player(gameEngine, name)
{
    commandProcessor_ = &commandProcessor;
}

/**
 * Copy Constructor
 */
Player::Player(const Player &other) : name{new std::string(*other.name)}, ordersList{other.ordersList},
    territories{new std::vector(*other.territories)}, reinforcementPool{new unsigned(*other.reinforcementPool)},
    cantTarget{new std::vector(*other.cantTarget)}, hand{new Hand(*other.hand)}
{
    gameEngine_ = other.gameEngine_;
    commandProcessor_ = other.commandProcessor_;
}

/**
 * Destructor
 */
Player::~Player() {
   delete name;
   delete ordersList;
   delete hand;
   delete territories;
   delete reinforcementPool;
   delete cantTarget;
}

std::string Player::getName() const {
    return *name;
}

void Player::changeName(const std::string &newName) {
    name = new std::string(newName);
}

// Add a method to add a territory to the player's collection
void Player::add(Territory &territory) const {
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

void Player::removeNegotiators() const {
    cantTarget->clear();
}

void Player::addNegotiator(Player negotiator) const
{
    cantTarget->push_back(&negotiator);
}

std::vector<const Territory *> Player::toDefend() const {
    std::vector<const Territory *> territoriesToDefend;

    // Iterate through the player's territories
    for (const auto territory: *territories) {
        // Get the adjacent territories of the current territory
        auto adjacentTerritories = gameEngine_->map().adjacencies(*territory);

        // Check if any adjacent territory is not owned by the player
        bool needsDefense = false;
        for (const auto adjacentTerritory: adjacentTerritories) {
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


std::vector<const Territory *> Player::toAttack() const
{
    std::vector<const Territory*> territoriesToAttack;

    // Iterate through the player's territories
    for (const auto myTerritory: *territories) {
        // Get the adjacent territories of the current territory

        // Check if adjacent territories are owned by other players
        for (auto territories = gameEngine_->map().adjacencies(*myTerritory); auto territory: territories) {
            if (&territory->owner() != this) {
                // Check that owner is not on cantAttack list because of negotiate order
                bool canAttack = true;
                for (const auto player : *cantTarget)
                {
                    if (&territory->owner() == &*player)
                    {
                        canAttack = false;
                        break;
                    }
                }

                // Check if adjacentTerritory is not already in territoriesToAttack
                bool isUnique = true;
                for (const auto attackTerritory : territoriesToAttack) {
                    if (attackTerritory->name() == territory->name()) {
                        isUnique = false;
                        break;
                    }
                }
                // If it's unique, add it to the list of territories to attack
                if (isUnique && canAttack)
                {
                    territoriesToAttack.push_back(territory);
                }
            }
        }
    }

    return territoriesToAttack;
}


void Player::issueOrders() const {
    // list available orders

    // get the command from the command line

    if (!commandProcessor_) return;

//    auto command = &commandProcessor_->getCommand(*gameEngine_);
//    ordersList->addOrder(command);

    //command->execute();


    //const std::string &orderType, Territory &source, Territory &target, unsigned armies, const Player &otherPlayer

//    if (orderType == "Deploy") {
//        // Create a DeployOrder object
//        ordersList->addOrder(DeployOrder{*this, target, armies, *gameEngine_});
//    } else if (orderType == "Advance") {
//        // Create an AdvanceOrder object
//        ordersList->addOrder(AdvanceOrder{*this, source, target, armies, *gameEngine_});
//    } else if (orderType == "Bomb") {
//        // Create a BombOrder object
//        ordersList->addOrder(BombOrder{*this, target, *gameEngine_});
//    } else if (orderType == "Blockade") {
//        // Create a BlockadeOrder object
//        ordersList->addOrder(BlockadeOrder{*this, target, *gameEngine_});
//    } else if (orderType == "Airlift") {
//        // Create an AirliftOrder object
//        ordersList->addOrder(AirliftOrder{*this, source, target, armies, *gameEngine_});
//    } else if (orderType == "Negotiate") {
//        // Create a NegotiateOrder object with the target player ID
//        ordersList->addOrder(NegotiateOrder{*this, otherPlayer, *gameEngine_});
//    } else {
//        return;
//    }
}

void Player::draw() const {
    gameEngine_->getDeck().draw(*hand);
}

void Player::play(const Card &card, Territory &target) {
    // Implement logic to play the card
    // Right now, I'll create a custom territory class
    gameEngine_->getDeck().discard(card.play(*this, target, *gameEngine_), *hand);
}
const Hand &Player::getHand() const {
    return *hand;
}

OrdersList &Player::orderList() const {
    return *ordersList;
}

void Player::fillReinforcementPool() const {
    *reinforcementPool += territoryBonusArmies() + continentBonusArmies();
}

unsigned int Player::territoryBonusArmies() const {
    return territories->size() >= 9 ? territories->size() / 3 : 3;
}

unsigned int Player::continentBonusArmies() const {
    unsigned armies = 0;
    const Map &map = gameEngine_->map();
    const auto p = *this;
    auto ownedBy = [p](const Territory *t) { return t->owner() == p; };
    for (const auto continent : map.continents()) {
        if (auto adj = map.adjacencies(*continent); adj.size() == std::ranges::count_if(adj, ownedBy))
            armies += continent->bonusArmies();
    }
    return armies;
}

bool Player::operator==(const Player &player) const {
    if (this == &player || *name == player.getName())
        return true;
    return false;
}
