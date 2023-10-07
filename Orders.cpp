// Orders.cpp

#include "Orders.h"
#include <iostream>
#include <algorithm>

// Daniel Soldera
// Carson Senthilkumar
// Joe El-Khoury
// Henri Stephane Carbon
// Haris Mahmood

//To do:
// Properly get the # of armies on source and target territories******
// Implement the subclasses of Order (NegotiateOrder logic)
const int MAX_TERRITORIES = 255; /*get the number for max territories in the given map*/
// Implement the Order class

Order::Order() {
    description = nullptr;
    effect = new std::string();
}
Order::Order(const Order& other) : description(new std::string(*(other.description))), effect(new std::string(*(other.effect))) {}
Order::~Order() {
    delete description;
    delete effect;
}
std::ostream& operator<<(std::ostream& os, const Order& order) {
    os << order.getDescription();
    if (!order.getEffect().empty()) {
        os << " (" << order.getEffect() << ")";
    }
    return os;
}

const std::string &Order::getDescription() const  {
    return *description;
}

const std::string &Order::getEffect() const {
    return *effect;
}

void Order::setDescription(const std::string &newDescription) {
    description = new std::string(newDescription);
}

void Order::setEffect(const std::string &newEffect) {
    effect = new std::string(newEffect);
}

Order &Order::operator=(const Order &order) {
    if (this != &order) {
        // clean up resources
        delete description;
        delete effect;

        description = new std::string(*order.description);
        effect = new std::string(*order.effect);
    }
    return *this;
}

// Implement DeployOrder class
DeployOrder::DeployOrder(int armies) : Order(), armies_to_deploy{new int(armies)} {
    description = new std::string("Deploy");
}

bool DeployOrder::validate() const {
    return *armies_to_deploy > 0;
}

void DeployOrder::execute() {
    if (validate()) {
        *effect = "Deployed " + std::to_string(*armies_to_deploy) + " armies.";
    }
}

DeployOrder::DeployOrder(const DeployOrder &order) : Order(order) {
    armies_to_deploy = new int(*order.armies_to_deploy);
}

DeployOrder::~DeployOrder() {
    delete armies_to_deploy;
    armies_to_deploy = nullptr;
}

DeployOrder &DeployOrder::operator=(const DeployOrder &order) {
    if (this != &order) {
        Order::operator=(order);
        delete armies_to_deploy;
        armies_to_deploy = new int(*order.armies_to_deploy);
    }
    return *this;
}

DeployOrder *DeployOrder::clone() const {
    return new DeployOrder(*this);
}

// Implement AdvanceOrder class
AdvanceOrder::AdvanceOrder(int source, int target, int armies) : Order(), armies_to_advance{new int(armies)}, target_territory{new int(target)}, source_territory{new int(source)} {
    description = new std::string("Advance");
}

bool AdvanceOrder::validate() const {
    // Check if source and target territories are valid
    if (*source_territory < 0 || *target_territory < 0) {
        return false;
    }

    // Check if the number of armies to advance is non-negative
    if (*armies_to_advance < 0) {
        return false;
    }

    return true;
}


void AdvanceOrder::execute() {
    if (validate()) {
        // Check if the source and target territories exist (replace with your logic)
        if (*source_territory < 0 || *target_territory < 0) {
            *effect = "Failed to execute AdvanceOrder: Invalid territories.";
            return;
        }

        // Check if the player has enough armies in the source territory to advance
        int armies_in_source_territory = /* Get the number of armies in the source territory */ 0;
        int armies_in_target_territory = /* Get the number of armies in the target territory */ 0;
        if (armies_in_source_territory < *armies_to_advance) {
            *effect = "Failed to execute AdvanceOrder: Insufficient armies in the source territory.";
            return;
        }
        //If AdvanceOrder is valid, armies in source territory are reduced and armies in target territory are increased
        armies_in_source_territory -= *armies_to_advance;
        armies_in_target_territory += *armies_to_advance;

        //Update the effect string to describe the action
        *effect = "Advanced " + std::to_string(*armies_to_advance) + " armies from territory "
                  + std::to_string(*source_territory) + " to territory " + std::to_string(*target_territory) + ".";
    }
}

AdvanceOrder::AdvanceOrder(const AdvanceOrder &order) : Order(order) {
    source_territory = new int(*order.source_territory);
    target_territory = new int(*order.target_territory);
    armies_to_advance = new int(*order.armies_to_advance);
}

AdvanceOrder::~AdvanceOrder() {
    delete source_territory;
    delete target_territory;
    delete armies_to_advance;
}

AdvanceOrder &AdvanceOrder::operator=(const AdvanceOrder &order) {
    if (this != &order) {
        Order::operator=(order);
        delete source_territory;
        delete target_territory;
        delete armies_to_advance;

        source_territory = new int(*order.source_territory);
        target_territory = new int(*order.target_territory);
        armies_to_advance = new int(*order.armies_to_advance);
    }
    return *this;
}

AdvanceOrder *AdvanceOrder::clone() const {
    return new AdvanceOrder(*this);
}


// Constructor for BombOrder
BombOrder::BombOrder(int targetTerritory) : Order(), target_territory{new int(targetTerritory)} {
    description = new std::string("Bomb");
}

// Validate method for BombOrder
bool BombOrder::validate() const {


    // Check if the targetTerritory is within a valid range:
    if (*target_territory < 0 || *target_territory >= MAX_TERRITORIES) {
        return false;
    }

    return true;
}

// Execute method for BombOrder
void BombOrder::execute() {
    if (validate()) {

        // Remove armies from the target territory:
        int armies_in_target_territory = /* Get the number of armies in the target territory */ 0;
        armies_in_target_territory /= 2;

        // Update the effect string to describe the action
        *effect = "Bombed territory " + std::to_string(*target_territory) + ".";
    }
}

BombOrder::BombOrder(const BombOrder &order) : Order(order) {
    target_territory = new int(*order.target_territory);
}

BombOrder::~BombOrder() {
    delete target_territory;
}

BombOrder &BombOrder::operator=(const BombOrder &order) {
    if (this != &order) {
        Order::operator=(order);
        delete target_territory;

        target_territory = new int(*order.target_territory);
    }
    return *this;
}

BombOrder *BombOrder::clone() const {
    return new BombOrder(*this);
}


// Constructor for BlockadeOrder
BlockadeOrder::BlockadeOrder(int target) : Order(), target_territory{new int(target)} {
    description = new std::string("Block");
}

bool BlockadeOrder::validate() const {


    // check if the targetTerritory is within a valid range:
    if (*target_territory < 0 || *target_territory >= MAX_TERRITORIES) {
        return false;
    }


    return true; //
}

// Execute method for BlockadeOrder
void BlockadeOrder::execute() {
    if (validate()) {

        // Triple armies from the target territory:
        int armies_in_target_territory = /* Get the number of armies  : Order()in the target territory */ 0;
        armies_in_target_territory *= 3;

        // Update the effect string to describe the action
        *effect = "Blocked territory " + std::to_string(*target_territory) + ".";
    }
}

BlockadeOrder::BlockadeOrder(const BlockadeOrder &order) : Order(order) {
    target_territory = new int(*order.target_territory);
}

BlockadeOrder::~BlockadeOrder() {
    delete target_territory;
}

BlockadeOrder &BlockadeOrder::operator=(const BlockadeOrder &order) {
    if (this != &order) {
        Order::operator=(order);
        delete target_territory;

        target_territory = new int(*order.target_territory);
    }
    return *this;
}

BlockadeOrder *BlockadeOrder::clone() const {
    return new BlockadeOrder(*this);
}


// Constructor for AirliftOrder
AirliftOrder::AirliftOrder(int armies, int source, int target) : Order(), target_territory{new int(target)}, armies_to_advance{new int(armies)}, source_territory{new int(source)} {
    description = new std::string("Airlift");
}

bool AirliftOrder::validate() const {
    // Check if the sourceTerritory and targetTerritory are within a valid range:
    if (*source_territory < 0 || *source_territory >= MAX_TERRITORIES ||
        *target_territory < 0 || *target_territory >= MAX_TERRITORIES) {

        return false;

    }

    // Check if the player has enough armies in the source territory to airlift
    int armies_in_source_territory = /* Get the number of armies in the source territory */ 10;
    if (armies_in_source_territory < *armies_to_advance) {

        return false;

    }

    return true;
}

// Execute method for AirliftOrder
void AirliftOrder::execute() {
    if (validate()) {

        // Reduce armies in source territory
        int armies_in_source_territory = /* Get the number of armies in the source territory */ 0;
        armies_in_source_territory -= *armies_to_advance;
        // Increase armies in source territory
        int armies_in_target_territory = /* Get the number of armies in the source territory */ 0;
        armies_in_target_territory += *armies_to_advance;

        // Update the effect string to describe the action
        *effect = "Airlifted " + std::to_string(*armies_to_advance) + " armies from territory "
                  + std::to_string(*source_territory) + " to territory " + std::to_string(*target_territory) + ".";
    }
}

AirliftOrder::AirliftOrder(const AirliftOrder &order) : Order(order) {
    source_territory = new int(*order.source_territory);
    target_territory = new int(*order.target_territory);
    armies_to_advance = new int(*order.armies_to_advance);
}

AirliftOrder::~AirliftOrder() {
    delete source_territory;
    delete target_territory;
    delete armies_to_advance;
}

AirliftOrder &AirliftOrder::operator=(const AirliftOrder &order) {
    if (this != &order) {
        Order::operator=(order);
        delete source_territory;
        delete target_territory;
        delete armies_to_advance;

        source_territory = new int(*order.source_territory);
        target_territory = new int(*order.target_territory);
        armies_to_advance = new int(*order.armies_to_advance);
    }
    return *this;
}

AirliftOrder *AirliftOrder::clone() const {
    return new AirliftOrder(*this);
}

// Constructor for NegotiateOrder
NegotiateOrder::NegotiateOrder(int targetPlayer) : Order(), target_player{new int(targetPlayer)} {
    description = new std::string("Negotiation");
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

        // the two players are now in a state of negotiation.
        // game_state.setNegotiation(target_player);

        // Update the effect string to describe the action
        *effect = "Initiated negotiation with player " + std::to_string(*target_player) + ".";
    }
}

NegotiateOrder::NegotiateOrder(const NegotiateOrder &order) : Order(order) {
    target_player = new int(*order.target_player);
}

NegotiateOrder::~NegotiateOrder() {
    delete target_player;
}

NegotiateOrder &NegotiateOrder::operator=(const NegotiateOrder &order) {
    if (this != &order) {
        Order::operator=(order);
        delete target_player;

        target_player = new int(*order.target_player);
    }
    return *this;
}

NegotiateOrder *NegotiateOrder::clone() const {
    return new NegotiateOrder(*this);
}


OrdersList::OrdersList() : orders{new std::vector<Order*>{}} {

}

OrdersList& OrdersList::addOrder(Order* order) {
    orders->push_back(order->clone()); // Store the pointer to the Order
    return *this;
}

OrdersList& OrdersList::remove(int index) {
    if (index >= 0 && index < orders->size()) {
        delete (*orders)[index]; // Delete the Order object
        orders->erase(orders->begin() + index); // Remove it from the vector
    }
    return *this;
}

OrdersList& OrdersList::move(int from, int to) {
    if (from >= 0 && from < orders->size() && to >= 0 && to < orders->size()) {
        std::swap(orders[from], orders[to]); // Swap the Order pointers
    }
//    else
//        std::cout<<"One or more indexes are out of range!";
    return *this;
}

OrdersList& OrdersList::executeOrders() {
    for (auto &order : *orders) {
        order->execute();
    }
    return *this;
}

std::vector<Order*> &OrdersList::getOrder() const {
    return *orders;
}

OrdersList::~OrdersList() {
    for (auto o: *orders) {
        delete o;
    }
    orders->clear();
    delete orders;
}

OrdersList::OrdersList(const OrdersList &ordersList) {
    orders = new std::vector<Order*>{};
    // create a deep copy
    for (auto & order : *ordersList.orders)
    {
        orders->push_back(order->clone());
    }
}

OrdersList &OrdersList::operator=(const OrdersList &ordersList) {
    if (this != &ordersList) {
        for (auto o: *orders) {
            delete o;
        }
        delete orders;
        orders = new std::vector<Order*>{};
        // create a deep copy
        for (auto & order : *ordersList.orders)
        {
            orders->push_back(order->clone());
        }

    }
    return *this;
}
