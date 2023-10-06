// Orders.cpp

#include "Orders.h"
#include <iostream>
#include <algorithm>
//To do:
// Properly get the # of armies on source and target territories******
// Implement the subclasses of Order (NegotiateOrder logic)
const int MAX_TERRITORIES = 255; /*get the number for max territories in the given map*/
// Implement the Order class
std::ostream& operator<<(std::ostream& os, const Order& order) {
    os << order.getDescription();
    if (!order.getEffect().empty()) {
        os << " (" << order.getEffect() << ")";
    }
    return os;
}

// Implement DeployOrder class
DeployOrder::DeployOrder(int armies) : armies_to_deploy{new int(armies)} {
    *description = "Deploy";
}

bool DeployOrder::validate() const {
    return *armies_to_deploy > 0;
}

void DeployOrder::execute() {
    if (validate()) {

        *effect = "Deployed " + std::to_string(*armies_to_deploy) + " armies.";
    }
}

// Implement AdvanceOrder class
AdvanceOrder::AdvanceOrder(int source, int target, int armies) : armies_to_advance{new int(armies)}, target_territory{new int(target)}, source_territory{new int(source)} {
    *description = "Advance";
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


// Constructor for BombOrder
BombOrder::BombOrder(int targetTerritory) : target_territory{new int(targetTerritory)} {
    *description = "Bomb";
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


// Constructor for BlockadeOrder
BlockadeOrder::BlockadeOrder(int target) : target_territory{new int(target)} {
    *description = "Block";
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
        int armies_in_target_territory = /* Get the number of armies in the target territory */ 0;
        armies_in_target_territory *= 3;

        // Update the effect string to describe the action
        *effect = "Blocked territory " + std::to_string(*target_territory) + ".";
    }
}


// Constructor for AirliftOrder
AirliftOrder::AirliftOrder(int armies, int source, int target) : target_territory{new int(target)}, armies_to_advance{new int(armies)}, source_territory{new int(source)} {
    *description = "Airlift";
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

// Constructor for NegotiateOrder
NegotiateOrder::NegotiateOrder(int targetPlayer) : target_player{new int(targetPlayer)} {
    *description = "Negotiation";
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


OrdersList::OrdersList() {
    // No need for dynamic allocation, just initialize the vector
}

OrdersList& OrdersList::addOrder(Order* order) {
    orders.push_back(order); // Store the pointer to the Order
    return *this;
}

OrdersList& OrdersList::remove(int index) {
    if (index >= 0 && index < orders.size()) {
        orders.erase(orders.begin() + index);
    }
    return *this;
}

OrdersList& OrdersList::move(int from, int to) {
    if (from >= 0 && from < orders.size() && to >= 0 && to < orders.size()) {
        std::swap(orders[from], orders[to]); // Swap the Order pointers
    }
//    else
//        std::cout<<"One or more indexes are out of range!";
    return *this;
}

OrdersList& OrdersList::executeOrders() {
    for (Order* order : orders) {
        order->execute();
    }
    return *this;
}

std::vector<Order*> OrdersList::getOrder(){
    return orders;
}


OrdersList::~OrdersList() {
    // Delete the dynamically allocated Order objects
    for (Order* order : orders) {
        delete order;
    }
    orders.clear(); // Clear the vector of pointers
}
//
// Created by Carson on 2023-10-05.
//
