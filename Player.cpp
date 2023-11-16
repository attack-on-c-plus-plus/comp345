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
#include "PlayerStrategies.h"

/**
 * Parameterized Constructor
 */
Player::Player(GameEngine&gameEngine, const std::string&name, const Strategy strategy) : name_{new std::string(name)},
    ordersList_{new OrdersList()}, territories_{new std::vector<Territory *>},
    hand_{new Hand()}, reinforcementPool_{new unsigned(50)}, cantTarget_{new std::vector<const Player *>},
    deployComplete_{new bool(false)}, ordersComplete_{new bool(false)}, strategy_{new Strategy(strategy)} {
    gameEngine_ = &gameEngine;
    playerStrategy_ = createStrategy(strategy);
}

/**
 * Copy Constructor
 */
Player::Player(const Player&player) : name_{new std::string(*player.name_)}, ordersList_{player.ordersList_},
                                      territories_{new std::vector(*player.territories_)},
                                      reinforcementPool_{new unsigned(*player.reinforcementPool_)},
                                      cantTarget_{new std::vector(*player.cantTarget_)}, hand_{new Hand(*player.hand_)},
                                      deployComplete_{new bool(*player.deployComplete_)},
                                      ordersComplete_{new bool(*player.ordersComplete_)},
                                      strategy_{new Strategy(*player.strategy_)} {
    gameEngine_ = player.gameEngine_;
    playerStrategy_ = createStrategy(*player.strategy_);
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
    delete playerStrategy_;
    delete strategy_;
}

Player& Player::operator=(const Player&player) {
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
        delete playerStrategy_;
        delete strategy_;
        name_ = new std::string(*player.name_);
        ordersList_ = new OrdersList(*player.ordersList_),
                territories_ = new std::vector(*player.territories_);
        reinforcementPool_ = new unsigned(*player.reinforcementPool_);
        cantTarget_ = new std::vector(*player.cantTarget_);
        hand_ = new Hand(*player.hand_);
        deployComplete_ = new bool(*player.deployComplete_);
        ordersComplete_ = new bool(*player.ordersComplete_);
        gameEngine_ = player.gameEngine_;
        strategy_ = new Strategy(*player.strategy_);
        playerStrategy_ = createStrategy(*player.strategy_);
    }
    return *this;
}

bool Player::operator==(const Player&player) const {
    if (this == &player || *name_ == player.name())
        return true;
    return false;
}

std::string Player::name() const {
    return *name_;
}

void Player::name(const std::string& newName) {
    name_ = new std::string(newName);
}

bool Player::isDeploying() const {
    return gameEngine_->state() == GameState::assignReinforcements && !*deployComplete_;
}

bool Player::isIssuingOrders() const {
    return gameEngine_->state() == GameState::issueOrders && !*ordersComplete_;
}

unsigned Player::reinforcementPool() const {
    return *reinforcementPool_;
}

Strategy Player::strategy() const {
    return *strategy_;
}

OrdersList& Player::orderList() const {
    return *ordersList_;
}

const Hand& Player::hand() const {
    return *hand_;
}

const std::vector<Territory *>& Player::territories() const {
    return *territories_;
}

std::vector<const Territory *> Player::toAttack() const {
    return playerStrategy_->toAttack();
}

std::vector<const Territory *> Player::toDefend() const {
    return playerStrategy_->toDefend();
}

const std::vector<const Player *>& Player::cantAttack() const {
    return *cantTarget_;
}

/**
 * \brief Issues multiple orders
 */
void Player::issueOrders() const {
    // get the command from the command line
    while (isDeploying() || isIssuingOrders()) {
        issueOrder();
    }
}

/**
 * \brief Issues one order
 */
void Player::issueOrder() const {
    if (!(isDeploying() || isIssuingOrders())) return;

    playerStrategy_->issueOrder();
}

/**
 * \brief Add a territory to the player's collection
 * \param territory
 */
void Player::add(Territory&territory) const {
    territories_->push_back(&territory);
    territory.owner(*this);
}

void Player::addNegotiator(const Player& negotiator) const {
    cantTarget_->push_back(&negotiator);
}

void Player::removeNegotiators() const {
    cantTarget_->clear();
}

void Player::draw() const {
    gameEngine_->getDeck().draw(*hand_);
}

void Player::play(const Card&card, Territory&target) {
    // Implement logic to play the card
    // Right now, I'll create a custom territory class
    gameEngine_->getDeck().discard(card.play(*this, target, *gameEngine_), *hand_);
}

void Player::fillReinforcementPool() const {
    *reinforcementPool_ += territoryBonusArmies() + continentBonusArmies();
}

void Player::deploy(const unsigned armies) const {
    if (armies > *reinforcementPool_)
        throw std::out_of_range("armies cannot be larger than pool of reinforcements");
    *reinforcementPool_ -= armies;
}

void Player::doneOrders() const {
    if (gameEngine_->state() == GameState::assignReinforcements) {
        *deployComplete_ = true;
        *ordersComplete_ = false;
    }
    if (gameEngine_->state() == GameState::issueOrders) {
        *deployComplete_ = false;
        *ordersComplete_ = true;
    }
}

unsigned int Player::continentBonusArmies() const {
    unsigned armies = 0;
    const Map&map = gameEngine_->map();
    const auto&p = *this;
    auto ownedBy = [&p](const Territory* t) { return t->owner() == p; };
    for (const auto continent: map.continents()) {
        if (auto adj = map.adjacencies(*continent); adj.size() == std::ranges::count_if(adj, ownedBy))
            armies += continent->bonusArmies();
    }
    return armies;
}

unsigned int Player::territoryBonusArmies() const {
    return territories_->size() >= 9 ? territories_->size() / 3 : 3;
}

PlayerStrategy* Player::createStrategy(const Strategy strategy) {
    PlayerStrategy* playerStrategy = nullptr;
    switch (strategy) {
        case Strategy::Neutral:
            playerStrategy = new NeutralPlayerStrategy(*this, *gameEngine_);
            break;
        case Strategy::Cheater:
            // TODO: Add cheater
            playerStrategy = new NeutralPlayerStrategy(*this, *gameEngine_);
            break;
        case Strategy::Human:
            playerStrategy = new HumanPlayerStrategy(*this, *gameEngine_);
            break;
        case Strategy::Aggressive:
            // TODO: Add aggressive
            playerStrategy = new NeutralPlayerStrategy(*this, *gameEngine_);
            break;
        case Strategy::Benevolent:
            // TODO: Add benevolent
            playerStrategy = new NeutralPlayerStrategy(*this, *gameEngine_);
            break;
    }
    return playerStrategy;
}

std::ostream& operator<<(std::ostream&os, const Player&player) {
    os << *player.name_;
    return os;
}
