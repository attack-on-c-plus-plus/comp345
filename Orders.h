// Orders.h
#ifndef COMP345_ORDERS_H
#define COMP345_ORDERS_H

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include "Map.h"
#include "Player.h"
#include "LoggingObserver.h"

// Daniel Soldera
// Carson Senthilkumar
// Joe El-Khoury
// Henri Stephane Carbon
// Haris Mahmood

// Forward declaration
class Player;
class Territory;
class GameEngine;

/**
 *
 */
class Order : public ILoggable, public Subject {
public:
    Order(const Player &player, const std::string &description, GameEngine &gameEngine);
    // Copy constructor
    Order(const Order &order);
    virtual ~Order();
    [[nodiscard]] virtual Order* clone() const = 0;
    [[nodiscard]] virtual bool validate() const = 0;
    virtual void execute();
    [[nodiscard]] const std::string &description() const;
    [[nodiscard]] const std::string &effect() const;
    Order &operator=(const Order&);
    virtual std::ostream &printTo(std::ostream& os) const;
    [[nodiscard]] std::string stringToLog() const override;
protected:
    const Player *player_; // weak ptr
    std::string *description_;
    std::string *effect_;
    GameEngine *gameEngine_;
private:
    friend std::ostream& operator<<(std::ostream& os, const Order& order);
};

//
/**
 * Deploy: put a certain number of army units on a target territory
 */
class DeployOrder : public Order {
public:
    explicit DeployOrder(const Player &player, Territory &target, unsigned , GameEngine &gameEngine);
    DeployOrder(const DeployOrder &order);
    ~DeployOrder() override;
    [[nodiscard]] DeployOrder* clone() const override;
    [[nodiscard]] bool validate() const override;
    void execute() override;
    DeployOrder &operator=(const DeployOrder&);
    std::ostream& printTo(std::ostream& os) const override;
private:
    Territory *target_;  // weak ptr
    unsigned *armies_;
};


/**
 * Advance: move a certain number of army units from a source territory to a target territory
 */
class AdvanceOrder : public Order {
public:
    explicit AdvanceOrder(const Player &player, Territory &source, Territory &target, unsigned armies,
                          GameEngine &gameEngine);
    AdvanceOrder(const AdvanceOrder &order);
    ~AdvanceOrder() override;
    [[nodiscard]] AdvanceOrder* clone() const override;
    [[nodiscard]] bool validate() const override;
    void execute() override;
    AdvanceOrder &operator=(const AdvanceOrder&);
     std::ostream& printTo(std::ostream& os) const override;
private:
    Territory *source_;  // weak ptr
    Territory *target_;  // weak ptr
    unsigned *armies_;
};


/**
 * Bomb: destroy half of the army units located on a target territory. This order can only be issued if
 * a player has the bomb card in their hand.
 */
class BombOrder : public Order {
public:
    explicit BombOrder(const Player &player, Territory &target, GameEngine &gameEngine);
    BombOrder(const BombOrder &order);
    ~BombOrder() override;
    [[nodiscard]] BombOrder* clone() const override;
    [[nodiscard]] bool validate() const override;
    void execute() override;
    BombOrder &operator=(const BombOrder&);
    std::ostream& printTo(std::ostream& os) const override;
private:
    Territory *target_; // weak ptr
};


/**
 * Blockade: Triple the number of army units on a target territory and make it a neutral territory. This
 * order can only be issued if a player has the blockade card in their hand.
 */
class BlockadeOrder : public Order {
public:
    explicit BlockadeOrder(const Player &player, Territory &target, GameEngine &gameEngine);
    BlockadeOrder(const BlockadeOrder &order);
    ~BlockadeOrder() override;
    [[nodiscard]] BlockadeOrder* clone() const override;
    [[nodiscard]] bool validate() const override;
    void execute() override;
    void execute(Player* neutralPlayer);
    BlockadeOrder &operator=(const BlockadeOrder&);
    std::ostream& printTo(std::ostream& os) const override;
private:
    Territory *target_; // weak ptr
};

/**
 * Airlift: advance a certain number of army units from one from source territory to a target
 * territory. This order can only be issued if a player has the airlift card in their hand.
 */
class AirliftOrder : public Order {
public:
    explicit AirliftOrder(const Player &player, Territory &source, Territory &target, unsigned armies,
                          GameEngine &gameEngine);
    AirliftOrder(const AirliftOrder &order);
    ~AirliftOrder() override;
    [[nodiscard]] AirliftOrder* clone() const override;
    [[nodiscard]] bool validate() const override;
    void execute() override;
    AirliftOrder &operator=(const AirliftOrder&);
    std::ostream& printTo(std::ostream& os) const override;
private:
    Territory *source_; // weak ptr
    Territory *target_; // weak ptr
    unsigned *armies_;
};

/**
 * Negotiate: prevent attacks between the current player and another target player until the end
 * of the turn. This order can only be issued if a player has the diplomacy card in their hand.
 */
class NegotiateOrder : public Order {
public:
    explicit NegotiateOrder(const Player &player, const Player &otherPlayer, GameEngine &gameEngine);
    NegotiateOrder(const NegotiateOrder &order);
    ~NegotiateOrder() override;
    [[nodiscard]] NegotiateOrder* clone() const override;
    [[nodiscard]] bool validate() const override;
    void execute() override;
    NegotiateOrder &operator=(const NegotiateOrder&);
    std::ostream& printTo(std::ostream& os) const override;
private:
    const Player *otherPlayer_; // weak ptr
};

/**
 * OrdersList class contains a list of Order objects
 */
class OrdersList : public ILoggable, public Subject {
public:
    OrdersList();
    OrdersList(const OrdersList&);
    ~OrdersList();

    OrdersList& addOrder(const Order &order);
    OrdersList& remove(int index);
    OrdersList& move(int from, int to);
    OrdersList& executeOrders();
    [[nodiscard]]const std::vector<Order*> &getOrder() const;
    [[nodiscard]] std::string stringToLog() const override;
    OrdersList &operator=(const OrdersList&);
private:
    std::vector<Order*> *orders_; // Store pointers to Order objects
};

#endif
