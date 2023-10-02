// Orders.cpp
#include "Orders.h"
#include <iostream>
//To do:
// Properly get the # of armies on source and target territories******
// Implement the subclasses of Order (NegotiateOrder logic) 

// Implement the Order class
std::ostream& operator<<(std::ostream& os, const Order& order) {
    os << order.description;
    if (!order.effect.empty()) {
        os << " (" << order.effect << ")";
    }
    return os;
}

// Implement DeployOrder class
DeployOrder::DeployOrder(int armies) : armies_to_deploy(armies) {
    description = "Deploy";
}

bool DeployOrder::validate() const {
    return armies_to_deploy > 0;
}

void DeployOrder::execute() {
    if (validate()) {

        effect = "Deployed " + std::to_string(armies_to_deploy) + " armies.";
    }
}

// Implement AdvanceOrder class 
AdvanceOrder::AdvanceOrder(int source, int target, int armies) : armies_to_advance(armies), target_territory(target), source_territory(source) {
    description = "Advance";
}

bool AdvanceOrder::validate() const {
    // Check if source and target territories are valid 
    if (source_territory < 0 || target_territory < 0) {
        return false;
    }

    // Check if the number of armies to advance is non-negative
    if (armies_to_advance < 0) {
        return false;
    }

    return true;
}


void AdvanceOrder::execute() {
    if (validate()) {
        // Check if the source and target territories exist (replace with your logic)
        if (source_territory < 0 || target_territory < 0) {
            effect = "Failed to execute AdvanceOrder: Invalid territories.";
            return;
        }

        // Check if the player has enough armies in the source territory to advance
        int armies_in_source_territory = /* Get the number of armies in the source territory */ 0;
        int armies_in_target_territory = /* Get the number of armies in the target territory */ 0;
        if (armies_in_source_territory < armies_to_advance) {
            effect = "Failed to execute AdvanceOrder: Insufficient armies in the source territory.";
            return;
        }
        //If AdvanceOrder is valid, armies in source territory are reduced and armies in target territory are increased 
        armies_in_source_territory -= armies_to_advance; 
        armies_in_target_territory += armies_to_advance;
        
        //Update the effect string to describe the action
        effect = "Advanced " + std::to_string(armies_to_advance) + " armies from territory "
                 + std::to_string(source_territory) + " to territory " + std::to_string(target_territory) + ".";
    }
}


// Constructor for BombOrder
BombOrder::BombOrder(int targetTerritory) : target_territory(targetTerritory) {
    description = "Bomb";
}

// Validate method for BombOrder
bool BombOrder::validate() const {
    
    int MAX_TERRITORIES = 255; /*get the number for max territories in the given map*/
    // Check if the targetTerritory is within a valid range:
    if (target_territory < 0 || target_territory >= MAX_TERRITORIES) {
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
        effect = "Bombed territory " + std::to_string(target_territory) + ".";
    }
}

bool BlockadeOrder::validate() const {
    
    int MAX_TERRITORIES = 255;
    // For example, you might check if the targetTerritory is within a valid range:
    if (target_territory < 0 || target_territory >= MAX_TERRITORIES) {
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
        effect = "Blocked territory " + std::to_string(target_territory) + ".";
    }
}


bool AirliftOrder::validate() const {

    // Return true if the order is valid, false otherwise.
    int MAX_TERRITORIES = 255;

    // Check if the sourceTerritory and targetTerritory are within a valid range:
    if (source_territory < 0 || source_territory >= MAX_TERRITORIES ||
        target_territory < 0 || target_territory >= MAX_TERRITORIES) {
        return false;
    }

    // Check if the player has enough armies in the source territory to airlift
    int armies_in_source_territory = /* Get the number of armies in the source territory */ 0;
    if (armies_in_source_territory < armies_to_advance) {
        return false;
    }

    return true; 
}

// Execute method for AirliftOrder
void AirliftOrder::execute() {
    if (validate()) {
        
        // Reduce armies in source territory
        int armies_in_source_territory = /* Get the number of armies in the source territory */ 0;
        armies_in_source_territory -= armies_to_advance;
        // Increase armies in source territory
        int armies_in_target_territory = /* Get the number of armies in the source territory */ 0;
        armies_in_target_territory += armies_to_advance;
        
        // Update the effect string to describe the action
        effect = "Airlifted " + std::to_string(armies_to_advance) + " armies from territory "
                 + std::to_string(source_territory) + " to territory " + std::to_string(target_territory) + ".";
    }
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
        effect = "Initiated negotiation with player " + std::to_string(target_player) + ".";
    }
}



// Implement OrdersList methods
void OrdersList::addOrder(Order* order) {
    orders.push_back(order);
}

void OrdersList::remove(int index) {
    if (index >= 0 && index < orders.size()) {
        delete orders[index];
        orders.erase(orders.begin() + index);
    }
}

void OrdersList::move(int from, int to) {
    if (from >= 0 && from < orders.size() && to >= 0 && to < orders.size()) {
        std::swap(orders[from], orders[to]);
    }
}

void OrdersList::executeOrders() {
    for (Order* order : orders) {
        order->execute();
    }
}







// Implement the OrdersDriver.cpp for testing
/*using namespace std;
  int main() {
    // Test your OrdersList and orders here
    OrdersList ordersList;

    // Create and add orders
    DeployOrder deployOrder(5);
    AdvanceOrder advanceOrder(1, 2, 3);

    ordersList.addOrder(&deployOrder);
    ordersList.addOrder(&advanceOrder);

    // Execute orders
    ordersList.executeOrders();

    // Print orders and their effects
    cout << ordersList << endl;

    // Remove and move orders as needed for testing

    return 0;
}*/