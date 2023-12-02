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

#include "Orders.h"

#include <algorithm>
#include <iostream>
#include <sstream>
#include <vector>

#include "Cards.h"
#include "CommandProcessing.h"
#include "GameEngine.h"
#include "Map.h"
#include "Player.h"
#include "PlayerStrategies.h"

// Implementation Order class

/**
 * Constructor
 * @param gameEngine
 * @param description
 * @param player
 */
Order::Order(const GameEngine &gameEngine, const std::string &description, const Player &player) {
    player_ = &player;
    description_ = new std::string(description);
    effect_ = new std::string();
    gameEngine_ = &gameEngine;
}

/**
 * Copy constructor
 * @param order
 */
Order::Order(const Order& order) :
    Subject(order),
    description_(new std::string(*order.description_)),
    effect_(new std::string(*order.effect_)) {
    player_ = order.player_;
    gameEngine_ = order.gameEngine_;
}

/**Random
 * Destructor
 */
Order::~Order() {
    delete description_;
    delete effect_;
}

const Player& Order::player() const {
    return *player_;
}

void Order::execute() {
    std::cout << *effect_ << std::endl;
    Notify(*this);
}

/**
 * Gets the order description
 * @return
 */
const std::string &Order::description() const  {
    return *description_;
}

/**
 * Operator= overload
 * @param order
 * @return
 */
Order &Order::operator=(const Order &order) {
    if (this != &order) {
        // clean up resources
        delete description_;
        delete effect_;

        description_ = new std::string(*order.description_);
        effect_ = new std::string(*order.effect_);
    }
    return *this;
}

std::string Order::stringToLog() const {
    std::stringstream s;
    s << "| Order Executed: ";
    s << *this;
    return s.str();
}

std::ostream &Order::printTo(std::ostream &os) const {
    if(!effect_->empty())
        os << "(" << player_->name() <<") - "<< *effect_;
    else
        os << *description_ << "(" << player_->name() << ") - ";
    return os;
}

/**
 * Operator<< overload
 * @param os
 * @param order
 * @return
 */
std::ostream& operator<<(std::ostream& os, const Order& order) {
    return order.printTo(os);
}

EndOrder::EndOrder(const GameEngine& gameEngine, Player& player) : Order(gameEngine, "Deploy", player) {
}

EndOrder::EndOrder(const EndOrder& order) = default;


Order& EndOrder::clone() const {
    return *new EndOrder(*this);
}

OrderType EndOrder::type() const {
    return OrderType::end;
}

bool EndOrder::validate() {
    if (!(gameEngine_->state() == GameState::assignReinforcements
        || gameEngine_->state() == GameState::issueOrders
        || gameEngine_->state() == GameState::executeOrders)) {
        return false;
    }
    return true;
}

void EndOrder::execute() {
    // Don't do anything, this is an end order
}

void EndOrder::attach(const Observer& observer) {
    // Don't do anything this should not log
}

void EndOrder::detach(const Observer& observer) {
    // Don't do anything this should not log
}

std::ostream& EndOrder::printTo(std::ostream& os) const {
    return os;
}

// Implementation DeployOrder class
/**
 * Constructor
 * @param gameEngine
 * @param player
 * @param target
 * @param armies
 */
DeployOrder::DeployOrder(const GameEngine &gameEngine, Player &player, Territory &target, const unsigned armies) :
        Order(gameEngine, "Deploy", player),
    armies_{new unsigned (armies)} {
    targetTerritory_ = &target;
}

/**
 * Copy constructor
 * @param order
 */
DeployOrder::DeployOrder(const DeployOrder &order) : Order(order) {
    armies_ = new unsigned(*order.armies_);
    targetTerritory_ = order.targetTerritory_;
}

/**
 * Destructor
 */
DeployOrder::~DeployOrder() {
    delete armies_;
}

/// \brief Clones a DeployOrder
/// \return a clone
Order& DeployOrder::clone() const {
    return *new DeployOrder(*this);
}

OrderType DeployOrder::type() const {
    return OrderType::deploy;
}

/**
 * Validates the DeployOrder
 * @return true if valid; false otherwise
 */
bool DeployOrder::validate() {
    if (gameEngine_->state() != GameState::assignReinforcements) {
        *effect_ = "Failed to execute DeployOrder: wrong game state";
        return false;
    }
    if (*armies_ > 0 && *armies_ <= player_->reinforcementPool()) {

        if (targetTerritory_->owner().name() == player_->name()) {
            *effect_ = "Succesfully deployed armies to target territory!";
            return true;
        }
        *effect_ = "Failed to execute DeployOrder: Target territory is not owned by the player.";
    }
    else {
        *effect_ = "Failed to execute DeployOrder: Number of armies to deploy must be greater than 0.";
    }
    return false;
}

/**
 * Executes the DeployOrder
 */
void DeployOrder::execute() {
    if (validate()) {
        std::stringstream ss;
        targetTerritory_->addArmies(*armies_);
        player_->deploy(*armies_);
        ss << "Deployed " << *armies_ << " armies on " << *targetTerritory_;
        *effect_ = ss.str();
    }
    Order::execute();
}

/**
 * Operator= overload
 * @param order
 * @return
 */
DeployOrder &DeployOrder::operator=(const DeployOrder &order) {
    if (this != &order) {
        Order::operator=(order);
        delete armies_;
        armies_ = new unsigned(*order.armies_);
    }
    return *this;
}

std::ostream &DeployOrder::printTo(std::ostream &os) const {
    if (effect_->empty())
    {
        return Order::printTo(os) << *armies_ << " armies to " << targetTerritory_;
    }
    return Order::printTo(os);

}

unsigned DeployOrder::armies() const {
    return *armies_;
}

// Implementation AdvanceOrder class
/**
 * Constructor
 * @param gameEngine
 * @param player
 * @param source
 * @param target
 * @param armies
 */
AdvanceOrder::AdvanceOrder(const GameEngine &gameEngine, Player &player, Territory &source, Territory &target,
                           const unsigned armies) :
        Order(gameEngine, "Advance", player),
    armies_{new unsigned(armies)} {
    sourceTerritory_ = &source;
    targetTerritory_ = &target;
}

/**
 * Copy constructor
 * @param order
 */
AdvanceOrder::AdvanceOrder(const AdvanceOrder &order) : Order(order) {
    sourceTerritory_ = order.sourceTerritory_;
    targetTerritory_ = order.targetTerritory_;
    armies_ = new unsigned(*order.armies_);
}

/**
 * Destructor
 */
AdvanceOrder::~AdvanceOrder() {
    // only delete non-weak ptr
    delete armies_;
}

/// \brief Clones an AdvanceOrder
/// \return a clone
Order& AdvanceOrder::clone() const {
    return *new AdvanceOrder(*this);
}

OrderType AdvanceOrder::type() const {
    return OrderType::advance;
}

/**
 * Validates the AdvanceOrder
 * @return true if valid; false otherwise
 */
bool AdvanceOrder::validate() {
    if (!(gameEngine_->state() == GameState::issueOrders || gameEngine_->state() == GameState::executeOrders)) {
        *effect_ = "Failed to execute Advance Order: wrong game state";
        return false;
    }

    for(const auto negotiators = player_->cantAttack(); const auto enemy : negotiators)
    {
        if (*enemy == targetTerritory_->owner()) {
            *effect_ = "Unable to attack a player negotiated with this turn";
            return false;
        }
    }

    // Check if the number of armies to advance is non-negative
    if(*armies_ < 0) {
        *effect_ = "Failed to execute AdvanceOrder: Number of armies to deploy must be greater than 0.";
        return false;
    }

    // If the player doesn't own source territory, order is invalid
    if (player_ != &sourceTerritory_->owner()){
        *effect_ = "Failed to execute AdvanceOrder: Player issuing Advance Order must own the source territory";
        return false;
    }

    // If target territory is not adjacent to the territory owned by the player
    // issuing the order, order is invalid
    for (const auto territory : player_->territories()) {
        if(auto adjacentTerritories = gameEngine_->map().adjacencies(*territory);
            std::ranges::find(adjacentTerritories, targetTerritory_) != adjacentTerritories.end()) {
            *effect_ = player_->name() + "Successfully played AdvanceOrder!";
            return true;
        }
    }

    *effect_ = "Failed to execute AdvanceOrder: target territory must be adjacent to the territory owned by the player issuing the order";
    return false;
}


void AdvanceOrder::execute() {
    if (validate()) {
        std::stringstream ss;
        sourceTerritory_->removeArmies(*armies_);

        //if player issuing order owns source territory but doesn't own target territory
        // ATTACK
        if (sourceTerritory_->owner() != targetTerritory_->owner()) {
            targetTerritory_->owner().attacked();

            while (targetTerritory_->armyCount() > 0 && *armies_ > 0) {
                const unsigned targetArmies = targetTerritory_->armyCount();
                // Each attacking army unit involved has 60% chances of killing one defending army
                for (int i = 0; i < *armies_; i++) {
                    if (const unsigned result1 = gameEngine_->random().generate(1, 10);
                        result1 <= 6 && targetTerritory_->armyCount() >= 1) {
                        targetTerritory_->removeArmies(1);
                    }
                }
                // Each defending army unit has 70% chances of killing one attacking army unit
                for (int i = 0; i < targetArmies; i++) {
                    if (const unsigned result2 = gameEngine_->random().generate(1, 10); result2 <= 7 && *armies_ >= 1) {
                        (*armies_)--;
                    }
                }
            }

            ss << player_->name() << " attacks " << *targetTerritory_ << std::endl;
            // Attackers won
            if (targetTerritory_->armyCount() == 0) {
                ss << *player_ << " won " << *targetTerritory_ << std::endl;
                targetTerritory_->owner().remove(*targetTerritory_);
                if (targetTerritory_->owner().territories().empty())
                    ss << targetTerritory_->owner() << " eliminated." << std::endl;
                targetTerritory_->owner().remove(*targetTerritory_);
                //Player issuing Attack claims target territory
                player_->add(*targetTerritory_);
                //A player receives a card at the end of his turn if they successfully conquered at least one territory during their turn
                player_->draw();
            }
            //Defenders won
            else {
                ss << *player_ << " failed to win " << *targetTerritory_ << std::endl;
            }
        }
        else {
            ss << *player_ << " moved " << *armies_ << " armies from " << *sourceTerritory_ << " to " << *targetTerritory_ << std::endl;
        }
        // add the armies to the territory (will be zero if player lost so no effect)
        targetTerritory_->addArmies(*armies_);
        *effect_ = ss.str();
    }
    Order::execute();
}

/**
 * Operator= overload
 * @param order
 * @return
 */
AdvanceOrder &AdvanceOrder::operator=(const AdvanceOrder &order) {
    if (this != &order) {
        Order::operator=(order);
        delete armies_;

        sourceTerritory_ = order.sourceTerritory_;
        targetTerritory_ = order.targetTerritory_;
        armies_ = new unsigned(*order.armies_);
    }
    return *this;
}

std::ostream &AdvanceOrder::printTo(std::ostream &os) const {
    if(effect_->empty())
    {
        return Order::printTo(os) << std::to_string(*armies_) << " armies from "
                                  << sourceTerritory_->name() << " to " << targetTerritory_->name();
    }

    return Order::printTo(os);
}

// Implementation BombOrder class
/**
 * Constructor
 * @param gameEngine
 * @param player
 * @param target
 */
BombOrder::BombOrder(const GameEngine &gameEngine, const Player &player, const Territory &target) :
        Order(gameEngine, "Bomb", player) {
    targetTerritory_ = &target;
}

/**
 * Copy constructor
 * @param order
 */
BombOrder::BombOrder(const BombOrder &order) : Order(order) {
    targetTerritory_ = order.targetTerritory_;
}

/**
 * Destructor
 */
BombOrder::~BombOrder() = default;

/**
 * \brief Clones a BombOrder
 * \return a clone
 */
Order& BombOrder::clone() const {
    return *new BombOrder(*this);
}

OrderType BombOrder::type() const {
    return OrderType::bomb;
}

/**
 * Validates the BombOrder
 * @return boolean
 */
bool BombOrder::validate() {
    // Pre-Condition Checks
    if (!(gameEngine_->state() == GameState::issueOrders || gameEngine_->state() == GameState::executeOrders)) {
        *effect_ = "Failed to execute BombOrder: wrong game state";
        return false;
    }

    // If the player they target is themselves, order is invalid
    if (player_ == &targetTerritory_->owner())
        return false;


    // If target territory is not adjacent to the territory owned by the player
    // issuing the order, order is invalid
    auto ts = player_->toAttack();
    const auto it = std::ranges::find(ts, targetTerritory_);
    return it != ts.end();
}

/**
 * Executes the BombOrder
 */
void BombOrder::execute() {
    if (validate()) {
        targetTerritory_->owner().attacked();
        // Removes half of the army count
        targetTerritory_->removeArmies(targetTerritory_->armyCount()/2);
        // Update the effect string to describe the action
        *effect_ = "Bombed territory " + targetTerritory_->name() + ".";
    }
    Order::execute();
}

/**
 * Operator= overload
 * @param order
 * @return
 */
BombOrder &BombOrder::operator=(const BombOrder &order) {
    if (this != &order) {
        Order::operator=(order);
        targetTerritory_ = order.targetTerritory_;
    }
    return *this;
}

std::ostream &BombOrder::printTo(std::ostream &os) const {
    if(effect_->empty())
    {
        return Order::printTo(os) << targetTerritory_->name();
    }

    return Order::printTo(os);
}


// Implementation BlockadeOrder class

/**
 * Constructor
 * @param gameEngine
 * @param player
 * @param target
 */
BlockadeOrder::BlockadeOrder(const GameEngine &gameEngine, const Player &player, Territory &target) :
        Order(gameEngine, "Blockade", player) {
    targetTerritory_ = &target;
}

/**
 * Copy constructor
 * @param order
 */
BlockadeOrder::BlockadeOrder(const BlockadeOrder &order) : Order(order) {
    targetTerritory_ = order.targetTerritory_;
}

/**
 * Destructor
 */
BlockadeOrder::~BlockadeOrder() = default;

/// \brief Clones an Blockade
/// \return a clone
Order& BlockadeOrder::clone() const {
    return *new BlockadeOrder(*this);
}

OrderType BlockadeOrder::type() const {
    return OrderType::blockade;
}

/**
 * Validates the BlockadeOrder
 * @return
 */
bool BlockadeOrder::validate() {
    if (!(gameEngine_->state() == GameState::issueOrders || gameEngine_->state() == GameState::executeOrders)) {
        *effect_ = "Failed to execute Blockade Order: wrong game state";
        return false;
    }

    return player_ == &targetTerritory_->owner();
}

/**
 * Executes the BlockadeOrder
 */
void BlockadeOrder::execute() {
    if (validate()) {
        // Double armies from the target territory i.e x + x = x * 2:
        targetTerritory_->addArmies(targetTerritory_->armyCount());
        // Update the effect string to describe the action
        *effect_ = "Blocked territory " + targetTerritory_->name() + ".";
        gameEngine_->players().neutral().add(*targetTerritory_);
        player_->remove(*targetTerritory_);
    }
    Order::execute();
}

/**
 * Operator= overload
 * @param order
 * @return
 */
BlockadeOrder &BlockadeOrder::operator=(const BlockadeOrder &order) {
    if (this != &order) {
        Order::operator=(order);
        targetTerritory_ = order.targetTerritory_;
    }
    return *this;
}

std::ostream &BlockadeOrder::printTo(std::ostream &os) const {
    if(effect_->empty()) {
        return Order::printTo(os) << targetTerritory_->name();
    }
    return Order::printTo(os);
}

// Implementation AirliftOrder
/**
 * Constructor
 * @param gameEngine
 * @param player
 * @param source
 * @param target
 * @param armies
 */
AirliftOrder::AirliftOrder(const GameEngine &gameEngine, const Player &player, Territory &source, Territory &target,
                           const unsigned armies) :
        Order(gameEngine, "Airlift", player),
    armies_{new unsigned(armies)} {
    sourceTerritory_ = &source;
    targetTerritory_ = &target;
}

/**
 * Copy constructor
 * @param order
 */
AirliftOrder::AirliftOrder(const AirliftOrder &order) :
        Order(order) {
    sourceTerritory_ = order.sourceTerritory_;
    targetTerritory_ = order.targetTerritory_;
    armies_ = new unsigned(*order.armies_);
}

/**
 * Destructor
 */
AirliftOrder::~AirliftOrder() {
    delete armies_;
}

/// \brief Clones an AirliftOrder
/// \return a clone
Order& AirliftOrder::clone() const {
    return *new AirliftOrder(*this);
}

OrderType AirliftOrder::type() const {
    return OrderType::airlift;
}

/**
 * Validates the AirliftOrder
 * @return
 */
bool AirliftOrder::validate() {
    if (!(gameEngine_->state() == GameState::issueOrders || gameEngine_->state() == GameState::executeOrders)) {
        *effect_ = "Failed to execute AirliftOrder: wrong game state";
        return false;
    }

    // Check if the player has enough armies in the source territory to airlift
    if (sourceTerritory_->armyCount() < *armies_) {
        *effect_ = "Failed to play AirliftOrder: not enough armies in the source territory to airlift...";
        return false;
    }

    //If the source or target territory does not belong to the player that issued the order, the order is invalid
    if (sourceTerritory_->owner() != targetTerritory_->owner()) {
        *effect_ = "Failed to play AirliftOrder: " + targetTerritory_->name() + " doesn't own the source and target territories!";
        return false;
    }

    return true;
}

/**
 * Executes the AirliftOrder
 */
void AirliftOrder::execute() {
    if (validate()) {

        // Reduce armies in source territory
        sourceTerritory_->removeArmies(*armies_);

        // Increase armies in source territory
        targetTerritory_->addArmies(*armies_);

        // Update the effect string to describe the action
        *effect_ = "Airlifted " + std::to_string(*armies_) + " armies from territory "
                   + sourceTerritory_->name() + " to territory " + targetTerritory_->name() + ".";
    }
    Order::execute();
}


/**
 * Operator= overload
 * @param order
 * @return
 */
AirliftOrder &AirliftOrder::operator=(const AirliftOrder &order) {
    if (this != &order) {
        Order::operator=(order);
        delete armies_;

        sourceTerritory_ = order.sourceTerritory_;
        targetTerritory_ = order.targetTerritory_;
        armies_ = new unsigned(*order.armies_);
    }
    return *this;
}

std::ostream &AirliftOrder::printTo(std::ostream &os) const {
    if(effect_->empty()) {
        return Order::printTo(os) << std::to_string(*armies_) + " armies from "
                                  << sourceTerritory_->name() << " to " << targetTerritory_->name();
    }

    return Order::printTo(os);
}

// Implementation NegotiateOrder class

/**
 * Constructor
 * @param gameEngine
 * @param player
 * @param otherPlayer
 */
NegotiateOrder::NegotiateOrder(const GameEngine &gameEngine, const Player &player, Player &otherPlayer) :
        Order(gameEngine, "Negotiation", player) {
    otherPlayer_ = &otherPlayer;
}

/**
 * Copy constructor
 * @param order
 */
NegotiateOrder::NegotiateOrder(const NegotiateOrder &order) : Order(order) {
    otherPlayer_ = order.otherPlayer_;
}

/**
 * Destructor
 */
NegotiateOrder::~NegotiateOrder() = default;

/// \brief Clones an NegotiateOrder
/// \return a clone
Order& NegotiateOrder::clone() const {
    return *new NegotiateOrder(*this);
}

OrderType NegotiateOrder::type() const {
    return OrderType::negotiate;
}

/**
 * Validates the NegotiateOrder
 * @return
 */
bool NegotiateOrder::validate() {
    if (!(gameEngine_->state() == GameState::issueOrders || gameEngine_->state() == GameState::executeOrders)) {
        *effect_ = "Failed to execute NegotiateOrder: wrong game state";
        return false;
    }
    return player_ != otherPlayer_ && gameEngine_->players().has(*otherPlayer_);
}

/**
 * Executes the NegotiateOrder
 */
void NegotiateOrder::execute() {
    if (validate()) {
        player_->addNegotiator(*otherPlayer_);
        otherPlayer_->addNegotiator(*player_);
        // Update the effect string to describe the action
        *effect_ = "Initiated negotiation with player " + otherPlayer_->name() + ".";
    }
    Order::execute();
}

/**
 * Operator= overload
 * @param order
 * @return
 */
NegotiateOrder &NegotiateOrder::operator=(const NegotiateOrder &order) {
    if (this != &order) {
        Order::operator=(order);
        otherPlayer_ = order.otherPlayer_;
    }
    return *this;
}

std::ostream &NegotiateOrder::printTo(std::ostream &os) const {
    if(effect_->empty())
    {
        return Order::printTo(os) << "with " << otherPlayer_->name();
    }
    return Order::printTo(os);
}

// Implementation OrdersList
/**
 * Constructor
 */
OrdersList::OrdersList() : orders_{new std::vector<Order*>{}} { }

/**
 * Copy constructor
 * @param ordersList
 */
OrdersList::OrdersList(const OrdersList &ordersList) : Subject(ordersList) {
    orders_ = new std::vector<Order*>{};
    // create a deep copy
    for (const auto order : *ordersList.orders_)
    {
        addOrder(*order);
    }
}

/**
 * Destructor
 */
OrdersList::~OrdersList() {
    for (const auto o: *orders_) {
        delete o;
    }
    orders_->clear();
    delete orders_;
}

/**
 * Adds an Order to the OrdersList
 * @param order
 * @return
 */
OrdersList& OrdersList::addOrder(const Order& order) {
    orders_->push_back(&order.clone()); // Store the pointer to the Order
    Notify(*this);
    return *this;
}

/**
 * Removes an Order from the OrdersList
 * @param index
 * @return
 */
OrdersList& OrdersList::remove(const int index) {
    if (index >= 0 && index < orders_->size()) {
        delete (*orders_)[index]; // Delete the Order object
        orders_->erase(orders_->begin() + index); // Remove it from the vector
    }
    return *this;
}

/**
 * Moves an Order in the OrdersList
 * @param from index
 * @param to index
 * @return
 */
OrdersList& OrdersList::move(const int from, const int to) {
    if (from >= 0 && from < orders_->size() && to >= 0 && to < orders_->size()) {
        const auto temp = (*orders_)[from];
        (*orders_)[from] = (*orders_)[to];
        (*orders_)[to] = temp;
    }
    return *this;
}

/**
 * Executes all the orders in the OrdersList
 * @return
 */
OrdersList& OrdersList::executeOrders() {
    for (const auto order : *orders_) {
        order->execute();
    }
    // clear orders
    for (const auto order : *orders_) {
        delete order;
    }
    orders_->clear();
    return *this;
}

bool OrdersList::done() const {
    return !orders_->empty() && orders_->back()->type() == OrderType::end;
}

/**
 * Returns a the list of orders
 * @return
 */
const std::vector<Order*> &OrdersList::orders() const {
    return *orders_;
}

/**
 * Operator= overload
 * @param ordersList
 * @return
 */
OrdersList &OrdersList::operator=(const OrdersList &ordersList) {
    if (this != &ordersList) {
        for (const auto o: *orders_) {
            delete o;
        }
        delete orders_;
        orders_ = new std::vector<Order*>{};
        // create a deep copy
        for (auto order : *ordersList.orders_)
        {
            orders_->push_back(order);
        }

    }
    return *this;
}

std::string OrdersList::stringToLog() const {
    std::stringstream s;

    if (!orders_->empty()) {
        s << "| Order Issued: ";
        orders_->back()->printTo(s);
    }
    return s.str();
}

std::ostream &operator<<(std::ostream &os, const OrdersList &ordersList) {
    os << "Orders [" << std::endl;
    for (const auto order : *ordersList.orders_) {
        os << "\t" << *order << std::endl;
    }
    os << "]";
    return os;
}

