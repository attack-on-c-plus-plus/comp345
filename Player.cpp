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
#include "CommandProcessing.h"
#include "GameEngine.h"
#include "Map.h"
#include "Orders.h"

/**
 * Parameterized Constructor
 */
Player::Player(GameEngine &gameEngine, const std::string &name) :
    name_{new std::string(name)}, ordersList_{new OrdersList()}, territories_{new std::vector<Territory *>},
    hand_{new Hand()}, reinforcementPool_{new unsigned(50)}, cantTarget_{new std::vector<Player *>},
    deployComplete_{new bool(false)}, ordersComplete_{new bool(false)}, commandProcessor_{new CommandProcessor()}
{
    gameEngine_ = &gameEngine;
}

/**
 * Copy Constructor
 */
Player::Player(const Player &player) : name_{new std::string(*player.name_)}, ordersList_{player.ordersList_},
    territories_{new std::vector(*player.territories_)}, reinforcementPool_{new unsigned(*player.reinforcementPool_)},
    cantTarget_{new std::vector(*player.cantTarget_)}, hand_{new Hand(*player.hand_)},
    deployComplete_{new bool(*player.deployComplete_)}, ordersComplete_{new bool(*player.ordersComplete_)},
    commandProcessor_{new CommandProcessor()}
{
    gameEngine_ = player.gameEngine_;
}

/**
 * Destructor
 */
Player::~Player() {
    delete name_;
    delete ordersList_;
    delete hand_;
    delete territories_;
    delete reinforcementPool_;
    delete cantTarget_;
    delete deployComplete_;
    delete ordersComplete_;
    delete commandProcessor_;
}

std::string Player::getName() const {
    return *name_;
}

void Player::changeName(const std::string &newName) {
    name_ = new std::string(newName);
}

// Add a method to add a territory to the player's collection
void Player::add(Territory &territory) const {
    territories_->push_back(&territory);
    territory.owner(*this);
}

const std::vector<Territory *> &Player::getTerritories() const {
    return *territories_;
}

const std::vector<Player *> &Player::getCantAttack() const
{
    return *cantTarget_;
}

void Player::removeNegotiators() const {
    cantTarget_->clear();
}

void Player::addNegotiator(Player negotiator) const
{
    cantTarget_->push_back(&negotiator);
}

std::vector<const Territory *> Player::toDefend() const {
    std::vector<const Territory *> territoriesToDefend;

    // Iterate through the player's territories
    for (const auto territory: *territories_) {
        // Get the adjacent territories of the current territory
        auto adjacentTerritories = gameEngine_->map().adjacencies(*territory);

        // Check if any adjacent territory is not owned by the player
        bool needsDefense = false;
        for (const auto adjacentTerritory: adjacentTerritories) {
            if (adjacentTerritory->owner().name_ != this->name_) {
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
    for (const auto myTerritory: *territories_) {
        // Check if adjacent territories are owned by other players
        for (auto territories = gameEngine_->map().adjacencies(*myTerritory); auto territory: territories) {
            if (&territory->owner() != this) {
                // Check that owner is not on cantAttack list because of negotiate order
                bool canAttack = true;
                for (const auto player : *cantTarget_)
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


bool Player::isDeploying() const {
    return gameEngine_->state() == GameState::assignReinforcements && !*deployComplete_;
}

bool Player::isIssuingOrders() const {
    return gameEngine_->state() == GameState::issueOrders && !*ordersComplete_;
}

/**
 * \brief Issues multiple orders
 */
void Player::issueOrders() {
    // list available orders

    if (!commandProcessor_) return;

    // get the command from the command line
    while (isDeploying() || isIssuingOrders()) {
        issueOrder();
    }
}

/**
 * \brief Issues one order
 */
void Player::issueOrder() {
    if (!(isDeploying() || isIssuingOrders())) return;

    if (Order *order = commandProcessor_->getOrder(*gameEngine_, *this)) {
        orderList().addOrder(*order);
        return;
    }
    if (gameEngine_->state() == GameState::assignReinforcements) { *deployComplete_ = true; *ordersComplete_ = false; }
    if (gameEngine_->state() == GameState::issueOrders) { *deployComplete_ = false; *ordersComplete_ = true; }
}

void Player::draw() const {
    gameEngine_->getDeck().draw(*hand_);
}

void Player::play(const Card &card, Territory &target) {
    // Implement logic to play the card
    // Right now, I'll create a custom territory class
    gameEngine_->getDeck().discard(card.play(*this, target, *gameEngine_), *hand_);
}
const Hand &Player::getHand() const {
    return *hand_;
}

const unsigned Player::reinforcementPool() const {
    return *reinforcementPool_;
}

Player& Player::operator=(const Player& player) {
    if (this != &player) {
        // This is used in case we are re-assigning to ourselves i.e. f = f;
        // We need to clean up the current pointers because destructor won't be called.
        delete name_;
        delete ordersList_;
        delete hand_;
        delete territories_;
        delete reinforcementPool_;
        delete cantTarget_;
        delete deployComplete_;
        delete ordersComplete_;
        delete commandProcessor_;
        name_ = new std::string(*player.name_);
        ordersList_ = new OrdersList(*player.ordersList_),
        territories_ = new std::vector(*player.territories_);
        reinforcementPool_ = new unsigned(*player.reinforcementPool_);
        cantTarget_ = new std::vector(*player.cantTarget_);
        hand_ = new Hand(*player.hand_);
        deployComplete_ = new bool(*player.deployComplete_);
        ordersComplete_ = new bool(*player.ordersComplete_);
        gameEngine_ = player.gameEngine_;
        commandProcessor_ = player.commandProcessor_;
    }
    return *this;
}

OrdersList &Player::orderList() const {
    return *ordersList_;
}

void Player::fillReinforcementPool() const {
    *reinforcementPool_ += territoryBonusArmies() + continentBonusArmies();
}

void Player::deploy(const unsigned armies) const {
    if (armies > *reinforcementPool_)
        throw std::out_of_range("armies cannot be larger than pool of reinforcements");
    *reinforcementPool_ -= armies;

}

unsigned int Player::territoryBonusArmies() const {
    return territories_->size() >= 9 ? territories_->size() / 3 : 3;
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
    if (this == &player || *name_ == player.getName())
        return true;
    return false;
}

std::ostream& operator<<(std::ostream& os, const Player& player) {
    os << *player.name_;
    return os;
}
