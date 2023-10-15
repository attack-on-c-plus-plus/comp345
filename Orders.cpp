// Orders.cpp

#include "Orders.h"
#include <iostream>
#include <algorithm>

// Daniel Soldera
// Carson Senthilkumar
// Joe El-Khoury
// Henri Stephane Carbon
// Haris Mahmood


// Implementation Order class

/**
 * Constructor
 * @param description
 */
Order::Order(const Player &player, const std::string &description) {
    player_ = &player;
    description_ = new std::string(description);
    effect_ = new std::string();
}

/**
 * Copy constructor
 * @param other
 */
Order::Order(const Order& other) :
    description_(new std::string(*(other.description_))),
    effect_(new std::string(*(other.effect_))) {
    player_ = other.player_;
}

/**
 * Destructor
 */
Order::~Order() {
    delete description_;
    delete effect_;
}

/**
 * Gets the order description
 * @return
 */
const std::string &Order::description() const  {
    return *description_;
}

/**
 * Gets the orders affect
 * @return
 */
const std::string &Order::effect() const {
    return *effect_;
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

/**
 * Operator<< overload
 * @param os
 * @param order
 * @return
 */
std::ostream& operator<<(std::ostream& os, const Order& order) {
    os << order.description();
    if (!order.effect().empty()) {
        os << " (" << order.effect() << ")";
    }
    return os;
}

// Implementation DeployOrder class
/**
 * Constructor
 * @param armies
 */
DeployOrder::DeployOrder(const Player &player, Territory &target, unsigned armies) :
    Order(player, "Deploy"),
    armies_{new unsigned (armies)} {
    target_ = &target;
}

/**
 * Copy constructor
 * @param order
 */
DeployOrder::DeployOrder(const DeployOrder &order) : Order(order) {
    armies_ = new unsigned(*order.armies_);
    target_ = order.target_;
}

/**
 * Destructor
 */
DeployOrder::~DeployOrder() {
    delete armies_;
}

/**
 * Validates the DeployOrder
 * @return true if valid; false otherwise
 */
bool DeployOrder::validate() const {
    return *armies_ > 0;
}

/**
 * Executes the DeployOrder
 */
void DeployOrder::execute() {
    if (validate()) {
        *effect_ = "Deployed " + std::to_string(*armies_) + " armies.";
    }
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

/**
 * Clones the DeployOrder
 * @return
 */
DeployOrder *DeployOrder::clone() const {
    return new DeployOrder(*this);
}

// Implementation AdvanceOrder class
/**
 * Constructor
 * @param source
 * @param target
 * @param armies
 */
AdvanceOrder::AdvanceOrder(const Player &player, Territory &source, Territory &target, unsigned armies) :
    Order(player, "Advance"),
    armies_{new unsigned(armies)} {
    source_ = &source;
    target_ = &target;
}

/**
 * Copy constructor
 * @param order
 */
AdvanceOrder::AdvanceOrder(const AdvanceOrder &order) : Order(order) {
    source_ = order.source_;
    target_ = order.target_;
    armies_ = new unsigned(*order.armies_);
}

/**
 * Destructor
 */
AdvanceOrder::~AdvanceOrder() {
    // only delete non-weak ptr
    delete armies_;
}

/**
 * Validates the AdvanceOrder
 * @return true if valid; false otherwise
 */
bool AdvanceOrder::validate() const {
    // Check if the number of armies to advance is non-negative

    // Check if the source and target territories exist
//    if (source_->getOwner() == *player_ || target_->getOwner() == *player_) {
//        *effect_ = "Failed to execute AdvanceOrder: Invalid territories.";
//        return false;
//    }


    return true;
}

/**
 * Executes the AdvanceOrder
 */
void AdvanceOrder::execute() {
    if (validate()) {
        // Check if the player has enough armies in the source territory to advance
        unsigned armies_in_source_territory = /* Get the number of armies in the source territory */ 0;
        unsigned armies_in_target_territory = /* Get the number of armies in the target territory */ 0;

        //If AdvanceOrder is valid, armies in source territory are reduced and armies in target territory are increased
        armies_in_source_territory -= *armies_;
        armies_in_target_territory += *armies_;

        //Update the effect string to describe the action
        *effect_ = "Advanced " + std::to_string(*armies_) + " armies from territory "
                   + source_->getName() + " to territory " + target_->getName() + ".";
    }
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

        source_ = order.source_;
        target_ = order.target_;
        armies_ = new unsigned(*order.armies_);
    }
    return *this;
}

/**
 * Clones the AdvanceOrder
 * @return
 */
AdvanceOrder *AdvanceOrder::clone() const {
    return new AdvanceOrder(*this);
}

// Implementation BombOrder class
/**
 * Constructor
 * @param targetTerritory
 */
BombOrder::BombOrder(const Player &player, Territory &target) :
    Order(player, "Bomb") {
    target_ = &target;
}

/**
 * Copy constructor
 * @param order
 */
BombOrder::BombOrder(const BombOrder &order) :
        Order(order) {
    target_ = order.target_;
}

/**
 * Destructor
 */
BombOrder::~BombOrder() = default;

/**
 * Validates the BombOrder
 * @return
 */
bool BombOrder::validate() const {
    // Check if the targetTerritory is within a valid range:
    return true;
}

/**
 * Executes the BombOrder
 */
void BombOrder::execute() {
    if (validate()) {
        // Remove armies from the target territory:
        int armies_in_target_territory = /* Get the number of armies in the target territory */ 0;
        armies_in_target_territory /= 2;

        // Update the effect string to describe the action
        *effect_ = "Bombed territory " + target_->getName() + ".";
    }
}

/**
 * Operator= overload
 * @param order
 * @return
 */
BombOrder &BombOrder::operator=(const BombOrder &order) {
    if (this != &order) {
        Order::operator=(order);
        target_ = order.target_;
    }
    return *this;
}

/**
 * Clones the BombOrder
 * @return
 */
BombOrder *BombOrder::clone() const {
    return new BombOrder(*this);
}


// Implementation BlockadeOrder class

/**
 * Constructor
 * @param target
 */
BlockadeOrder::BlockadeOrder(const Player &player, Territory &target) :
    Order(player, "Block") {
    target_ = &target;
}

/**
 * Copy constructor
 * @param order
 */
BlockadeOrder::BlockadeOrder(const BlockadeOrder &order) : Order(order) {
    target_ = order.target_;
}

/**
 * Destructor
 */
BlockadeOrder::~BlockadeOrder() = default;

/**
 * Validates the BlockadeOrder
 * @return
 */
bool BlockadeOrder::validate() const {
    return true; //
}

/**
 * Executes the BlockadeOrder
 */
void BlockadeOrder::execute() {
    if (validate()) {

        // Triple armies from the target territory:
        unsigned armies_in_target_territory = /* Get the number of armies  : Order()in the target territory */ 0;
        armies_in_target_territory *= 3;

        // Update the effect string to describe the action
        *effect_ = "Blocked territory " + target_->getName() + ".";
    }
}

/**
 * Operator= overload
 * @param order
 * @return
 */
BlockadeOrder &BlockadeOrder::operator=(const BlockadeOrder &order) {
    if (this != &order) {
        Order::operator=(order);
        target_ = order.target_;
    }
    return *this;
}

/**
 * Clones the BlockadeOrder
 * @return
 */
BlockadeOrder *BlockadeOrder::clone() const {
    return new BlockadeOrder(*this);
}


// Implementation AirliftOrder
/**
 * Constructor
 * @param source
 * @param target
 * @param armies
 */
AirliftOrder::AirliftOrder(const Player &player, Territory &source, Territory &target, unsigned armies) :
    Order(player, "Airlift"),
    armies_{new unsigned(armies)} {
    source_ = &source;
    target_ = &target;
}

/**
 * Copy constructor
 * @param order
 */
AirliftOrder::AirliftOrder(const AirliftOrder &order) :
        Order(order) {
    source_ = order.source_;
    target_ = order.target_;
    armies_ = new unsigned(*order.armies_);
}

/**
 * Destructor
 */
AirliftOrder::~AirliftOrder() {
    delete armies_;
}

/**
 * Validates the AirliftOrder
 * @return
 */
bool AirliftOrder::validate() const {
    // Check if the player has enough armies in the source territory to airlift
    unsigned armies_in_source_territory = /* Get the number of armies in the source territory */ 10;
    if (armies_in_source_territory < *armies_) {

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
        unsigned armies_in_source_territory = /* Get the number of armies in the source territory */ 0;
        armies_in_source_territory -= *armies_;
        // Increase armies in source territory
        unsigned armies_in_target_territory = /* Get the number of armies in the source territory */ 0;
        armies_in_target_territory += *armies_;

        // Update the effect string to describe the action
        *effect_ = "Airlifted " + std::to_string(*armies_) + " armies from territory "
                   + source_->getName() + " to territory " + target_->getName() + ".";
    }
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

        source_ = order.source_;
        target_ = order.target_;
        armies_ = new unsigned(*order.armies_);
    }
    return *this;
}

/**
 * Clones the AirliftOrder
 * @return
 */
AirliftOrder *AirliftOrder::clone() const {
    return new AirliftOrder(*this);
}

// Implementation NegotiateOrder class
/**
 * Constructor
 * @param player
 * @param otherPlayer
 */
NegotiateOrder::NegotiateOrder(const Player &player, const Player &otherPlayer) :
    Order(player,"Negotiation") {
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

/**
 * Validates the NegotiateOrder
 * @return
 */
bool NegotiateOrder::validate() const {

    // check if the target player exists:
    bool playerExists = /*!game_state.playerExists(target_player)*/ false;
    if (playerExists) {
        return false;
    }

    // check if player has a negotiate card ?

    return true; // Replace with actual validation logic
}

/**
 * Executes the NegotiateOrder
 */
void NegotiateOrder::execute() {
    if (validate()) {

        // Update the effect string to describe the action
        *effect_ = "Initiated negotiation with player " + otherPlayer_->getName() + ".";
    }
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

/**
 * Clones the NegotiateOrder
 * @return
 */
NegotiateOrder *NegotiateOrder::clone() const {
    return new NegotiateOrder(*this);
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
OrdersList::OrdersList(const OrdersList &ordersList) {
    orders_ = new std::vector<Order*>{};
    // create a deep copy
    for (auto & order : *ordersList.orders_)
    {
        orders_->push_back(order->clone());
    }
}

/**
 * Destructor
 */
OrdersList::~OrdersList() {
    for (auto o: *orders_) {
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
    orders_->push_back(order.clone()); // Store the pointer to the Order
    return *this;
}

/**
 * Removes an Order from the OrdersList
 * @param index
 * @return
 */
OrdersList& OrdersList::remove(int index) {
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
OrdersList& OrdersList::move(int from, int to) {
    if (from >= 0 && from < orders_->size() && to >= 0 && to < orders_->size()) {
        auto temp = (*orders_)[from];
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
    for (auto &order : *orders_) {
        order->execute();
    }
    return *this;
}

/**
 * Returns a the list of orders
 * @return
 */
const std::vector<Order*> &OrdersList::getOrder() const {
    return *orders_;
}

/**
 * Operator= overload
 * @param ordersList
 * @return
 */
OrdersList &OrdersList::operator=(const OrdersList &ordersList) {
    if (this != &ordersList) {
        for (auto o: *orders_) {
            delete o;
        }
        delete orders_;
        orders_ = new std::vector<Order*>{};
        // create a deep copy
        for (auto & order : *ordersList.orders_)
        {
            orders_->push_back(order->clone());
        }

    }
    return *this;
}
