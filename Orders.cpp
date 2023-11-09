// Orders.cpp

#include "Orders.h"
#include "Cards.h"
#include <iostream>
#include <algorithm>
#include <sstream>
#include <random>
#include <vector>
using namespace std;

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
Order::Order(const Player &player, const std::string &description, GameEngine &gameEngine) {
    player_ = &player;
    description_ = new std::string(description);
    effect_ = new std::string();
    gameEngine_ = &gameEngine;
}

/**
 * Copy constructor
 * @param other
 */
Order::Order(const Order& other) :
    Subject(other),
    description_(new std::string(*(other.description_))),
    effect_(new std::string(*(other.effect_))) {
    player_ = other.player_;
    gameEngine_ = other.gameEngine_;
}

/**
 * Destructor
 */
Order::~Order() {
    delete description_;
    delete effect_;
}

void Order::execute() {
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

std::string Order::stringToLog() const {
    std::stringstream s;
    s << "| Order Executed: ";
    s << *this;
    return s.str();
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

std::ostream &Order::printTo(std::ostream &os) const {
    os << *description_ << " (" << player_->getName() << ") - ";
    return os;
}

// Implementation DeployOrder class
/**
 * Constructor
 * @param armies
 */
DeployOrder::DeployOrder(const Player &player, Territory &target, unsigned armies, GameEngine &gameEngine) :
    Order(player, "Deploy", gameEngine),
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
    
    if (*armies_ > 0) {

        if (target_->owner().getName() == player_->getName()) {
            *effect_ = "Succesfully deployed armies to target territory!";
            return true;
        } 
        else {
            *effect_ = "Failed to execute DeployOrder: Target territory is not owned by the player.";
        }
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
        target_->addArmies(*armies_);
        *effect_ = "Deployed " + std::to_string(*armies_) + " armies on " + (target_->name()) + "!";
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

/**
 * Clones the DeployOrder
 * @return
 */
DeployOrder *DeployOrder::clone() const {
    return new DeployOrder(*this);
}

std::ostream &DeployOrder::printTo(std::ostream &os) const {
    return Order::printTo(os) << std::to_string(*armies_) << " armies to " << target_->name();
}

// Implementation AdvanceOrder class
/**
 * Constructor
 * @param source
 * @param target
 * @param armies
 */
AdvanceOrder::AdvanceOrder(const Player &player, Territory &source, Territory &target, unsigned armies,
                           GameEngine &gameEngine) :
    Order(player, "Advance", gameEngine),
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
    if(*armies_ < 0){
        *effect_ = "Failed to execute AdvanceOrder: Number of armies to deploy must be greater than 0.";
        return false;
    }

    // If the player doesn't own source territory, order is invalid
    if (player_ != &source_->owner()){
        *effect_ = "Failed to execute AdvanceOrder: Player issuing Advance Order must own the source territory";
        return false;
    }
    // Get the player's adjacent territories
    auto * playerTerritories = new std::vector<Territory*>(player_->getTerritories());

    Map map = gameEngine_->map();

    // If target territory is not adjacent to the territory owned by the player
    // issuing the order, order is invalid
    bool isValid = false;
    for (Territory *territory : *playerTerritories) {
        auto * adjacentTerritories = new std::vector<const Territory*> (map.adjacencies(*territory));
        if(std::find(adjacentTerritories->begin(), adjacentTerritories->end(), target_) != adjacentTerritories->end()) {
            isValid = true;
            delete adjacentTerritories;
            break;
        }
        delete adjacentTerritories;
    }

    delete playerTerritories;
    if(isValid){
        *effect_ = (player_->getName()) + "Successfully played AdvanceOrder!";
        return true;
    }
    else{
        *effect_ = "Failed to execute AdvanceOrder: target territory must be adjacent to the territory owned by the player issuing the order";
        return false;
    }
    
}
    
    
void AdvanceOrder::execute() {

    if (validate()) {
        
        //if player issuing order owns target and source territory
        if(source_->owner().getName() == target_->owner().getName()){
            //If AdvanceOrder is valid, armies in source territory are reduced and armies in target territory are increased
            source_->removeArmies(*armies_);
            target_->addArmies(*armies_);

            //Update the effect string to describe the action
            *effect_ = player_->getName() + "succesfully Advanced " + std::to_string(*armies_) + " armies from territory "
                    + source_->name() + " to territory " + target_->name() + ".";
        }

        //if player issuing order owns source territory but doesn't own target territory
        //ATTACK
        if(source_->owner().getName() != target_->owner().getName()){
            int remainingAttackers = *armies_; // used of attackers win
            std::default_random_engine generator;
            std::uniform_int_distribution<int> distribution(1, 100);
            std::cout << " --> Initiating attack!" << std::endl;

            // initiating attack: loops through number of armies for each territory.
            // each army has a probability to kill off the other, the first territory that reaches zero armies left loses.
            int result1{}, result2{};
            while (target_->armyCount() > 0 && *armies_ > 0) {
                  int targetArmies = target_->armyCount();
                 // Each attacking army unit involved has 60% chances of killing one defending army
                 for (int i = 0; i < *armies_; i++) {
                     result1 = distribution(generator);
                     if (result1 <= 60) {
                     target_->removeArmies(1);
                     }
                    }
                  // Each defending army unit has 70% chances of killing one attacking army unit
                 for (int i = 0; i < targetArmies; i++) {
                     result2 = distribution(generator);
                     if (result2 <= 70){
                        *armies_ -= 1;
                        remainingAttackers--;
                     } 
                 }
             }


             //Attackers won
             if(target_->armyCount() == 0){
                //Player issuing Attack claims target territory
                target_->owner(*player_);
                //The attacking army units that survived the battle then occupy the conquered territory.
                target_->addArmies(remainingAttackers);
                //A player receives a card at the end of his turn if they successfully conquered at least one territory during their turn
                Player(*player_).drawCardFromDeck(gameEngine_->getDeck());

                std::cout << " --> Attackers won! " + player_->getName() + " now owns the target territory."  << std::endl;
             }
             //Defenders won
             else{
                //remove armies sent from source territory
                source_->removeArmies(*armies_);
                std::cout << " --> Defenders won! " + player_->getName() + " lost the battle for " +  target_->name() + "..." << std::endl;
             }

        }
            //Update the effect string to describe the action
            *effect_ = player_->getName() + "succesfully Advanced " + std::to_string(*armies_) + " armies from territory "
                    + source_->name() + " to territory " + target_->name() + ".";
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

std::ostream &AdvanceOrder::printTo(std::ostream &os) const {
    return Order::printTo(os) << std::to_string(*armies_) << " armies from "
        << source_->name() << " to " << target_->name();
}

// Implementation BombOrder class
/**
 * Constructor
 * @param targetTerritory
 */
BombOrder::BombOrder(const Player &player, Territory &target, GameEngine &gameEngine) :
    Order(player, "Bomb", gameEngine) {
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
 * @param map
 * @return boolean
 */
bool BombOrder::validate() const {
    bool isValid = false;
    // Pre-Condition Checks

    // If the player they target is themselves, order is invalid
    if (player_ == &target_->owner())
        return isValid;

    // Get the player's adjacent territories
    auto * playerTerritories = new std::vector<Territory*>(player_->getTerritories());

    Map map = gameEngine_->map();

    // If target territory is not adjacent to the territory owned by the player
    // issuing the order, order is invalid
    for (Territory *territory : *playerTerritories) {
        auto * adjacentTerritories = new std::vector<const Territory*> (map.adjacencies(*territory));
        if(std::find(adjacentTerritories->begin(), adjacentTerritories->end(), target_) != adjacentTerritories->end()) {
            isValid = true;
            delete adjacentTerritories;
            break;
        }
        delete adjacentTerritories;
    }

    delete playerTerritories;
    return isValid;
}

/**
 * Executes the BombOrder
 */
void BombOrder::execute() {
    if (validate()) {
        // Remove armies from the target territory:
        int armies_in_target = target_->armyCount();
        int updated_armies_in_target = armies_in_target/2;
        //So now we want to reduce the number of armies in target territory by half i.e armies-armies/2 = armies/2
        target_->removeArmies(updated_armies_in_target);

        // Update the effect string to describe the action
        *effect_ = "Bombed territory " + target_->name() + ".";
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

std::ostream &BombOrder::printTo(std::ostream &os) const {
    return Order::printTo(os) << target_->name();
}


// Implementation BlockadeOrder class

/**
 * Constructor
 * @param target
 */
BlockadeOrder::BlockadeOrder(const Player &player, Territory &target, GameEngine &gameEngine) :
    Order(player, "Blockade", gameEngine) {
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
    if (player_ == &target_->owner()) {
        return true;
    } else {
        return false;
    }
}

/**
 * Executes the BlockadeOrder
 */
void BlockadeOrder::execute() {
    if (validate()) {

        // Double armies from the target territory i.e x + x = x * 2:
        unsigned armies_in_target_territory = target_->armyCount();
        target_->addArmies(armies_in_target_territory);

        // Update the effect string to describe the action
        *effect_ = "Blocked territory " + target_->name() + ".";

        bool neutralFound = false;
        for (Player *player : gameEngine_->getPlayers()) {
            if (player->getName() == "Neutral") {
                target_->owner(*player);
                neutralFound = true;
                break;
            }
        }
        if (!neutralFound) {
            auto * player = new Player("Netural");
            gameEngine_->getPlayers().push_back(player);
        }
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

std::ostream &BlockadeOrder::printTo(std::ostream &os) const {
    return Order::printTo(os) << target_->name();
}


// Implementation AirliftOrder
/**
 * Constructor
 * @param source
 * @param target
 * @param armies
 */
AirliftOrder::AirliftOrder(const Player &player, Territory &source, Territory &target, unsigned armies,
                           GameEngine &gameEngine) :
    Order(player, "Airlift", gameEngine),
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
    if (source_->armyCount() < *armies_) {
        *effect_ = "Failed to play AirliftOrder: not enough armies in the source territory to airlift...";
        return false;
    }

    //If the source or target territory does not belong to the player that issued the order, the order is invalid
    if ((source_->owner().getName() != target_->owner().getName()) && (player_->getName() != source_->owner().getName())) {
        *effect_ = "Failed to play AirliftOrder: " + target_->name() + " doesn't own the source and target territories!";
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
        source_->removeArmies(*armies_);
         
        // Increase armies in source territory
        target_->addArmies(*armies_);

        // Update the effect string to describe the action
        *effect_ = "Airlifted " + std::to_string(*armies_) + " armies from territory "
                   + source_->name() + " to territory " + target_->name() + ".";
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

std::ostream &AirliftOrder::printTo(std::ostream &os) const {
    return Order::printTo(os) << std::to_string(*armies_) + " armies from "
        << source_->name() << " to " << target_->name();
}

// Implementation NegotiateOrder class
/**
 * Constructor
 * @param player
 * @param otherPlayer
 */
NegotiateOrder::NegotiateOrder(const Player &player, const Player &otherPlayer, GameEngine &gameEngine) :
    Order(player,"Negotiation",gameEngine) {
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
    if (player_ == otherPlayer_)
    {
        return false;
    }

    std::vector<Player *> ourPlayers = gameEngine_->getPlayers();
    // check if the target player exists:
    bool playerExists = /*!game_state.playerExists(target_player)*/ false;
    for (Player *i : ourPlayers)
    {
        if (i == otherPlayer_)
        {
            playerExists = true;
        }
    }

    // check if player has a negotiate card ?
    if (playerExists) {
        std::vector<const Card *> ourHand = player_->getHand().cards();
        Card const card5{CardType::diplomacy};
        for(Card const *card : ourHand)
        {
            if (*card == card5)
            {
                return true;
            }
        }
        return false;
    }

    return false;
}

/**
 * Executes the NegotiateOrder
 */
void NegotiateOrder::execute() {
    if (validate()) {
        player_->addNegotiator(*otherPlayer_);
        otherPlayer_->addNegotiator(*player_);
        // Update the effect string to describe the action
        *effect_ = "Initiated negotiation with player " + otherPlayer_->getName() + ".";
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

/**
 * Clones the NegotiateOrder
 * @return
 */
NegotiateOrder *NegotiateOrder::clone() const {
    return new NegotiateOrder(*this);
}

std::ostream &NegotiateOrder::printTo(std::ostream &os) const {
    return Order::printTo(os) << "with " << otherPlayer_->getName();
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
    Notify(*this);
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
    for (auto order : *ordersList.orders_) {
        os << "\t" << *order << std::endl;
    }
    os << "]";
    return os;
}
