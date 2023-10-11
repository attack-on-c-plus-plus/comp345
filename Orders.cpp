// Orders.cpp

#include "Orders.h"
#include <iostream>
#include <algorithm>

// Daniel Soldera
// Carson Senthilkumar
// Joe El-Khoury
// Henri Stephane Carbon
// Haris Mahmood


// Implement the Order class

//Order::Order() : Order("") {}

Order::Order(const std::string &description) {
    description_ = new std::string(description);
    effect_ = new std::string();
}

Order::Order(const Order& other) :
        description_(new std::string(*(other.description_))),
        effect_(new std::string(*(other.effect_))) {}

Order::~Order() {
    delete description_;
    delete effect_;
}

const std::string &Order::description() const  {
    return *description_;
}

const std::string &Order::effect() const {
    return *effect_;
}

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

std::ostream& operator<<(std::ostream& os, const Order& order) {
    os << order.description();
    if (!order.effect().empty()) {
        os << " (" << order.effect() << ")";
    }
    return os;
}

// Implement DeployOrder class
DeployOrder::DeployOrder(unsigned armies) :
    Order("Deploy"),
    armies_{new unsigned (armies)} {
}

// Validate method for DeployOrder
bool DeployOrder::validate() const {
    return *armies_ > 0;
}

// Execute order for DeployOrder
void DeployOrder::execute() {
    if (validate()) {
        *effect_ = "Deployed " + std::to_string(*armies_) + " armies.";
    }
}

DeployOrder::DeployOrder(const DeployOrder &order) : Order(order) {
    armies_ = new unsigned(*order.armies_);
}

DeployOrder::~DeployOrder() {
    delete armies_;
}

DeployOrder &DeployOrder::operator=(const DeployOrder &order) {
    if (this != &order) {
        Order::operator=(order);
        delete armies_;
        armies_ = new unsigned(*order.armies_);
    }
    return *this;
}

DeployOrder *DeployOrder::clone() const {
    return new DeployOrder(*this);
}

// Implement AdvanceOrder class
AdvanceOrder::AdvanceOrder(size_t source, size_t target, unsigned armies) :
    Order("Advance"),
    sourceTerritory_{new size_t(source)},
    targetTerritory_{new size_t(target)},
    armies_{new unsigned(armies)} {

}

// Validate method for AdvanceOrder
bool AdvanceOrder::validate() const {
    // Check if the number of armies to advance is non-negative

    if (*armies_ < 0) {
        return false;
    }
//    if (armies_source_territory < *armies_) {
//        *effect_ = "Failed to execute AdvanceOrder: Insufficient armies in the source territory.";
//        return;
//    }


    // Check if the source and target territories exist
    if (*sourceTerritory_ < 0 || *targetTerritory_ < 0) {
        *effect_ = "Failed to execute AdvanceOrder: Invalid territories.";
        return false;
    }


    return true;
}

// Execute method for AdvanceOrder
void AdvanceOrder::execute() {
    if (validate()) {
        // Check if the player has enough armies in the source territory to advance
        int armies_in_source_territory = /* Get the number of armies in the source territory */ 0;
        int armies_in_target_territory = /* Get the number of armies in the target territory */ 0;

        //If AdvanceOrder is valid, armies in source territory are reduced and armies in target territory are increased
        armies_in_source_territory -= *armies_;
        armies_in_target_territory += *armies_;

        //Update the effect string to describe the action
        *effect_ = "Advanced " + std::to_string(*armies_) + " armies from territory "
                   + std::to_string(*sourceTerritory_) + " to territory " + std::to_string(*targetTerritory_) + ".";
    }
}

AdvanceOrder::AdvanceOrder(const AdvanceOrder &order) : Order(order) {
    sourceTerritory_ = new size_t(*order.sourceTerritory_);
    targetTerritory_ = new size_t(*order.targetTerritory_);
    armies_ = new unsigned(*order.armies_);
}

AdvanceOrder::~AdvanceOrder() {
    delete sourceTerritory_;
    delete targetTerritory_;
    delete armies_;
}

AdvanceOrder &AdvanceOrder::operator=(const AdvanceOrder &order) {
    if (this != &order) {
        Order::operator=(order);
        delete sourceTerritory_;
        delete targetTerritory_;
        delete armies_;

        sourceTerritory_ = new size_t(*order.sourceTerritory_);
        targetTerritory_ = new size_t(*order.targetTerritory_);
        armies_ = new unsigned(*order.armies_);
    }
    return *this;
}

AdvanceOrder *AdvanceOrder::clone() const {
    return new AdvanceOrder(*this);
}


// Constructor for BombOrder
BombOrder::BombOrder(size_t targetTerritory) :
    Order("Bomb"),
    targetTerritory_{new size_t(targetTerritory)} {
}

// Validate method for BombOrder
bool BombOrder::validate() const {
    // Check if the targetTerritory is within a valid range:
    return true;
}

// Execute method for BombOrder
void BombOrder::execute() {
    if (validate()) {
        // Remove armies from the target territory:
        int armies_in_target_territory = /* Get the number of armies in the target territory */ 0;
        armies_in_target_territory /= 2;

        // Update the effect string to describe the action
        *effect_ = "Bombed territory " + std::to_string(*targetTerritory_) + ".";
    }
}

BombOrder::BombOrder(const BombOrder &order) :
    Order(order) {
    targetTerritory_ = new size_t(*order.targetTerritory_);
}

BombOrder::~BombOrder() {
    delete targetTerritory_;
}

BombOrder &BombOrder::operator=(const BombOrder &order) {
    if (this != &order) {
        Order::operator=(order);
        delete targetTerritory_;

        targetTerritory_ = new size_t(*order.targetTerritory_);
    }
    return *this;
}

BombOrder *BombOrder::clone() const {
    return new BombOrder(*this);
}


// Constructor for BlockadeOrder
BlockadeOrder::BlockadeOrder(size_t target) : Order("Block"),
    targetTerritory_{new size_t(target)} {
}

// Validate method for BlockadeOrder
bool BlockadeOrder::validate() const {
    return true; //
}

// Execute method for BlockadeOrder
void BlockadeOrder::execute() {
    if (validate()) {

        // Triple armies from the target territory:
        int armies_in_target_territory = /* Get the number of armies  : Order()in the target territory */ 0;
        armies_in_target_territory *= 3;

        // Update the effect string to describe the action
        *effect_ = "Blocked territory " + std::to_string(*targetTerritory_) + ".";
    }
}

BlockadeOrder::BlockadeOrder(const BlockadeOrder &order) : Order(order) {
    targetTerritory_ = new size_t(*order.targetTerritory_);
}

BlockadeOrder::~BlockadeOrder() {
    delete targetTerritory_;
}

BlockadeOrder &BlockadeOrder::operator=(const BlockadeOrder &order) {
    if (this != &order) {
        Order::operator=(order);
        delete targetTerritory_;

        targetTerritory_ = new size_t(*order.targetTerritory_);
    }
    return *this;
}

BlockadeOrder *BlockadeOrder::clone() const {
    return new BlockadeOrder(*this);
}


// Constructor for AirliftOrder
AirliftOrder::AirliftOrder(size_t source, size_t target, unsigned armies) :
    Order("Airlift"),
    sourceTerritory_{new size_t(source)},
    targetTerritory_{new size_t(target)},
    armies_{new unsigned(armies)} {
}

// Validate method for AirliftOrder
bool AirliftOrder::validate() const {
    // Check if the player has enough armies in the source territory to airlift
    int armies_in_source_territory = /* Get the number of armies in the source territory */ 10;
    if (armies_in_source_territory < *armies_) {

        return false;

    }

    return true;
}

// Execute method for AirliftOrder
void AirliftOrder::execute() {
    if (validate()) {

        // Reduce armies in source territory
        int armies_in_source_territory = /* Get the number of armies in the source territory */ 0;
        armies_in_source_territory -= *armies_;
        // Increase armies in source territory
        int armies_in_target_territory = /* Get the number of armies in the source territory */ 0;
        armies_in_target_territory += *armies_;

        // Update the effect string to describe the action
        *effect_ = "Airlifted " + std::to_string(*armies_) + " armies from territory "
                   + std::to_string(*sourceTerritory_) + " to territory " + std::to_string(*targetTerritory_) + ".";
    }
}

AirliftOrder::AirliftOrder(const AirliftOrder &order) :
    Order(order) {
    sourceTerritory_ = new size_t(*order.sourceTerritory_);
    targetTerritory_ = new size_t(*order.targetTerritory_);
    armies_ = new unsigned(*order.armies_);
}

AirliftOrder::~AirliftOrder() {
    delete sourceTerritory_;
    delete targetTerritory_;
    delete armies_;
}

AirliftOrder &AirliftOrder::operator=(const AirliftOrder &order) {
    if (this != &order) {
        Order::operator=(order);
        delete sourceTerritory_;
        delete targetTerritory_;
        delete armies_;

        sourceTerritory_ = new size_t(*order.sourceTerritory_);
        targetTerritory_ = new size_t(*order.targetTerritory_);
        armies_ = new unsigned(*order.armies_);
    }
    return *this;
}

AirliftOrder *AirliftOrder::clone() const {
    return new AirliftOrder(*this);
}

// Constructor for NegotiateOrder
NegotiateOrder::NegotiateOrder(unsigned targetPlayer) : Order("Negotiation"),
    targetPlayer_{new unsigned(targetPlayer)} {
}

// Validate method for NegotiateOrder
bool NegotiateOrder::validate() const {

    // check if the target player exists:
    bool playerExists = /*!game_state.playerExists(target_player)*/ false;
    if (playerExists) {
        return false;
    }

    // check if player has negotiate card ?

    return true; // Replace with actual validation logic
}

// Execute method for NegotiateOrder
void NegotiateOrder::execute() {
    if (validate()) {
        
        // Update the effect string to describe the action
        *effect_ = "Initiated negotiation with player " + std::to_string(*targetPlayer_) + ".";
    }
}

NegotiateOrder::NegotiateOrder(const NegotiateOrder &order) : Order(order) {
    targetPlayer_ = new unsigned(*order.targetPlayer_);
}

NegotiateOrder::~NegotiateOrder() {
    delete targetPlayer_;
}

NegotiateOrder &NegotiateOrder::operator=(const NegotiateOrder &order) {
    if (this != &order) {
        Order::operator=(order);
        delete targetPlayer_;

        targetPlayer_ = new unsigned(*order.targetPlayer_);
    }
    return *this;
}

NegotiateOrder *NegotiateOrder::clone() const {
    return new NegotiateOrder(*this);
}


OrdersList::OrdersList() : orders_{new std::vector<Order*>{}} {

}

OrdersList& OrdersList::addOrder(Order* order) {
    orders_->push_back(order->clone()); // Store the pointer to the Order
    return *this;
}

OrdersList& OrdersList::remove(int index) {
    if (index >= 0 && index < orders_->size()) {
        delete (*orders_)[index]; // Delete the Order object
        orders_->erase(orders_->begin() + index); // Remove it from the vector
    }
    return *this;
}

OrdersList& OrdersList::move(int from, int to) {
    if (from >= 0 && from < orders_->size() && to >= 0 && to < orders_->size()) {
        auto temp = (*orders_)[from];
        (*orders_)[from] = (*orders_)[to];
        (*orders_)[to] = temp;
    }

    return *this;
}

OrdersList& OrdersList::executeOrders() {
    for (auto &order : *orders_) {
        order->execute();
    }
    return *this;
}

std::vector<Order*> &OrdersList::getOrder() const {
    return *orders_;
}

OrdersList::~OrdersList() {
    for (auto o: *orders_) {
        delete o;
    }
    orders_->clear();
    delete orders_;
}

OrdersList::OrdersList(const OrdersList &ordersList) {
    orders_ = new std::vector<Order*>{};
    // create a deep copy
    for (auto & order : *ordersList.orders_)
    {
        orders_->push_back(order->clone());
    }
}

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
