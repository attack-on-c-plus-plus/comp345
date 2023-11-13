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
#include "GameEngine.h"
#include "Map.h"
#include "Player.h"

// Implementation Order class

/**
 * Constructor
 * @param gameEngine
 * @param description
 * @param player
 */
Order::Order(GameEngine &gameEngine, const std::string &description, Player &player) {
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
    if(!effect_->empty())
    {
        os <<"("<<player_->getName() <<") - "<< *effect_;
    }

    else
    {
        os << *description_ << "(" << player_->getName() << ") - ";
    }
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
DeployOrder::DeployOrder(GameEngine &gameEngine, Player &player, Territory &target, const unsigned armies) :
        Order(gameEngine, "Deploy", player),
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
bool DeployOrder::validate() {
    if (gameEngine_->state() != GameState::assignReinforcements) {
        *effect_ = "Failed to execute DeployOrder: wrong game state";
        return false;
    }
    if (*armies_ > 0 && *armies_ <= player_->reinforcementPool()) {

        if (target_->owner().getName() == player_->getName()) {
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
        target_->addArmies(*armies_);
        player_->deploy(*armies_);
        *effect_ = "Deployed " + std::to_string(*armies_) + " armies on " + target_->name() + "!";
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
    if(effect_->empty())
    {
        return Order::printTo(os) << std::to_string(*armies_) << " armies to " << target_->name();
    }
    return Order::printTo(os);

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
AdvanceOrder::AdvanceOrder(GameEngine &gameEngine, Player &player, Territory &source, Territory &target,
                           const unsigned armies) :
        Order(gameEngine, "Advance", player),
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
bool AdvanceOrder::validate() {
    if (!(gameEngine_->state() == GameState::issueOrders || gameEngine_->state() == GameState::executeOrders)) {
        *effect_ = "Failed to execute Advance Order: wrong game state";
        return false;
    }

    for(const auto negotiators = player_->getCantAttack(); auto enemy : negotiators)
    {
        if (*enemy == target_->owner())
        {
            *effect_ = "Unable to attack a player negotiated with this turn";
            return false;
        }
    }

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
    const auto playerTerritories = player_->getTerritories();

    // If target territory is not adjacent to the territory owned by the player
    // issuing the order, order is invalid
    bool isValid = false;
    for (const auto territory : playerTerritories) {
        if(auto adjacentTerritories = gameEngine_->map().adjacencies(*territory);
            std::ranges::find(adjacentTerritories, target_) != adjacentTerritories.end()) {
            isValid = true;
            break;
        }
    }

    if(isValid){
        const auto name = player_->getName();
        *effect_ = name + "Successfully played AdvanceOrder!";
        return true;
    }
    *effect_ = "Failed to execute AdvanceOrder: target territory must be adjacent to the territory owned by the player issuing the order";


    return false;
}


void AdvanceOrder::execute() {

    if (validate()) {

        //if player issuing order owns target and source territory
        if(source_->owner().getName() == target_->owner().getName())
        {
            //If AdvanceOrder is valid, armies in source territory are reduced and armies in target territory are increased
            source_->removeArmies(*armies_);
            target_->addArmies(*armies_);

            //Update the effect string to describe the action
            *effect_ = player_->getName() + "succesfully Advanced " + std::to_string(*armies_) + " armies from territory "
                    + source_->name() + " to territory " + target_->name() + ".";
        }

        //if player issuing order owns source territory but doesn't own target territory
        //ATTACK
        unsigned originalAttackers=*armies_;
        if(source_->owner().getName() != target_->owner().getName()){
            unsigned remainingAttackers = *armies_; // used of attackers win
            Random random;
            std::cout << " --> Initiating attack!" << std::endl;

            while (target_->armyCount() > 0 && *armies_ > 0)
            {
                const unsigned targetArmies = target_->armyCount();
                // Each attacking army unit involved has 60% chances of killing one defending army
                for (int i = 0; i < *armies_; i++)
                {
                    if (const int result1 = random.generate(1,10); result1 <= 6 && target_->armyCount()>=1)
                    {
                        target_->removeArmies(1);
                    }
                }
                // Each defending army unit has 70% chances of killing one attacking army unit
                for (int i = 0; i < targetArmies; i++) {
                    if (const int result2 = random.generate(1,10); result2 <= 7 && *armies_>=1)
                    {
                        *armies_ -= 1;
                        remainingAttackers--;
                    }
                }
            }
            //Attackers won
            if(target_->armyCount() == 0) {
                //Player issuing Attack claims target territory
                target_->owner(*player_);
                //The attacking army units that survived the battle then occupy the conquered territory.
                target_->addArmies(remainingAttackers);
                //A player receives a card at the end of his turn if they successfully conquered at least one territory during their turn
                player_->draw();
                source_->removeArmies(originalAttackers);

                std::cout << " --> Attackers won! " + player_->getName() + " now owns the target territory."  << std::endl;
            }
            //Defenders won
            else {
                //remove armies sent from source territory
                source_->removeArmies(originalAttackers);
                std::cout << " --> Defenders won! " + player_->getName() + " lost the battle for " +  target_->name() + "..." << std::endl;
            }
        }

        //Update the effect string to describe the action
        *effect_ = player_->getName() + " Advanced " + std::to_string(*armies_) + " armies from territory "
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

std::ostream &AdvanceOrder::printTo(std::ostream &os) const {
    if(effect_->empty())
    {
        return Order::printTo(os) << std::to_string(*armies_) << " armies from "
                                  << source_->name() << " to " << target_->name();
    }

    return Order::printTo(os);
}

// Implementation BombOrder class
/**
 * Constructor
 * @param targetTerritory
 */
BombOrder::BombOrder(GameEngine &gameEngine, Player &player, Territory &target) :
        Order(gameEngine, "Bomb", player) {
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
 * @return boolean
 */
bool BombOrder::validate() {
    bool isValid = false;
    // Pre-Condition Checks
    if (!(gameEngine_->state() == GameState::issueOrders || gameEngine_->state() == GameState::executeOrders)) {
        *effect_ = "Failed to execute BombOrder: wrong game state";
        return false;
    }

    // If the player they target is themselves, order is invalid
    if (player_ == &target_->owner())
        return isValid;

    // Get the player's adjacent territories

    // If target territory is not adjacent to the territory owned by the player
    // issuing the order, order is invalid
    for (const auto playerTerritories = player_->getTerritories(); const auto territory : playerTerritories) {
        if(auto adjacentTerritories = gameEngine_->map().adjacencies(*territory);
            std::ranges::find(adjacentTerritories, target_) != adjacentTerritories.end()) {
            isValid = true;
            break;
        }
    }

    return isValid;
}

/**
 * Executes the BombOrder
 */
void BombOrder::execute() {
    if (validate()) {
        // Remove armies from the target territory:
        const unsigned armies_in_target = target_->armyCount();
        const unsigned updated_armies_in_target = armies_in_target/2;
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

std::ostream &BombOrder::printTo(std::ostream &os) const {
    if(effect_->empty())
    {
        return Order::printTo(os) << target_->name();
    }

    return Order::printTo(os);
}


// Implementation BlockadeOrder class

/**
 * Constructor
 * @param target
 */
BlockadeOrder::BlockadeOrder(GameEngine &gameEngine, Player &player, Territory &target) :
        Order(gameEngine, "Blockade", player) {
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
bool BlockadeOrder::validate() {
    if (!(gameEngine_->state() == GameState::issueOrders || gameEngine_->state() == GameState::executeOrders)) {
        *effect_ = "Failed to execute Blockade Order: wrong game state";
        return false;
    }

    if (player_ == &target_->owner()) {
        return true;
    }
    return false;
}

/**
 * Executes the BlockadeOrder
 */
void BlockadeOrder::execute() {
    if (validate()) {

        // Double armies from the target territory i.e x + x = x * 2:
        const unsigned int armies_in_target_territory = target_->armyCount();
        target_->addArmies(armies_in_target_territory);

        // Update the effect string to describe the action
        *effect_ = "Blocked territory " + target_->name() + ".";

        bool neutralFound = false;
        for (const Player *player : gameEngine_->getPlayers()) {
            if (player->getName() == "Neutral") {
                target_->owner(*player);
                neutralFound = true;
                break;
            }
        }
        if (!neutralFound) {
            auto * player = new Player(*gameEngine_, "Neutral");
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

std::ostream &BlockadeOrder::printTo(std::ostream &os) const {
    if(effect_->empty()) {
        return Order::printTo(os) << target_->name();
    }

    return Order::printTo(os);
}

// Implementation AirliftOrder
/**
 * Constructor
 * @param source
 * @param target
 * @param armies
 */
AirliftOrder::AirliftOrder(GameEngine &gameEngine, Player &player, Territory &source, Territory &target,
                           const unsigned armies) :
        Order(gameEngine, "Airlift", player),
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
bool AirliftOrder::validate() {
    if (!(gameEngine_->state() == GameState::issueOrders || gameEngine_->state() == GameState::executeOrders)) {
        *effect_ = "Failed to execute AirliftOrder: wrong game state";
        return false;
    }

    // Check if the player has enough armies in the source territory to airlift
    if (source_->armyCount() < *armies_) {
        *effect_ = "Failed to play AirliftOrder: not enough armies in the source territory to airlift...";
        return false;
    }

    //If the source or target territory does not belong to the player that issued the order, the order is invalid
    if (source_->owner().getName() != target_->owner().getName() && player_->getName() != source_->owner().getName()) {
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

std::ostream &AirliftOrder::printTo(std::ostream &os) const {
    if(effect_->empty()) {
        return Order::printTo(os) << std::to_string(*armies_) + " armies from "
                                  << source_->name() << " to " << target_->name();
    }

    return Order::printTo(os);
}

// Implementation NegotiateOrder class
/**
 * Constructor
 * @param player
 * @param otherPlayer
 */
NegotiateOrder::NegotiateOrder(GameEngine &gameEngine, Player &player, Player &otherPlayer) :
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

/**
 * Validates the NegotiateOrder
 * @return
 */
bool NegotiateOrder::validate() {
    if (!(gameEngine_->state() == GameState::issueOrders || gameEngine_->state() == GameState::executeOrders)) {
        *effect_ = "Failed to execute NegotiateOrder: wrong game state";
        return false;
    }

    if (player_ == otherPlayer_)
    {
        std::cout<<"Checking players";
        return false;
    }

    const std::vector<Player *> ourPlayers = gameEngine_->getPlayers();
    // check if the target player exists:
    bool playerExists = false;
    for (const Player *i : ourPlayers)
    {
        if (i == otherPlayer_)
        {
            playerExists = true;
        }
    }

    // check if player has a negotiate card ?
    if (playerExists) {
        for (const std::vector<const Card *> ourHand = player_->getHand().cards(); auto *card: ourHand) {
            if (card->type() == CardType::diplomacy) return true;
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

std::ostream &NegotiateOrder::printTo(std::ostream &os) const {
    if(effect_->empty())
    {
        return Order::printTo(os) << "with " << otherPlayer_->getName();
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
    for (auto order : *ordersList.orders_)
    {
        orders_->push_back(order);
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
OrdersList& OrdersList::addOrder(Order& order) {
    orders_->push_back(&order); // Store the pointer to the Order
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

