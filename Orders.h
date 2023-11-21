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

#ifndef COMP345_ORDERS_H
#define COMP345_ORDERS_H

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

#include "LoggingObserver.h"

enum class OrderType;
/**
 * Forward declaration
 */
class Player;
class Territory;
class GameEngine;

/**
 *
 */
class Order : public ILoggable, public Subject {
public:
    explicit Order(const GameEngine &gameEngine, const std::string &description, const Player &player);
    // Copy constructor
    Order(const Order &order);
    ~Order() override;
    [[nodiscard]] virtual Order &clone() const = 0;
    [[nodiscard]] virtual bool validate() = 0;
    [[nodiscard]] const Player &player() const;
    [[nodiscard]] virtual OrderType type() const = 0;
    virtual void execute();
    [[nodiscard]] virtual const std::string &description() const;
    Order &operator=(const Order&);
    virtual std::ostream &printTo(std::ostream& os) const;
    [[nodiscard]] std::string stringToLog() const override;
protected:
    const Player *player_; // weak ptr
    std::string *description_;
    std::string *effect_;
    const GameEngine *gameEngine_;
private:
    friend std::ostream& operator<<(std::ostream& os, const Order& order);
};

class EndOrder final : public Order {
public:
    explicit EndOrder(const GameEngine &gameEngine, Player &player);
    EndOrder(const EndOrder &order);
    ~EndOrder() override = default;
    [[nodiscard]] Order& clone() const override;
    [[nodiscard]] OrderType type() const override;
    [[nodiscard]] bool validate() override;
    void execute() override;
    void attach(const Observer& observer) override;
    void detach(const Observer& observer) override;
    std::ostream& printTo(std::ostream& os) const override;
};

//
/**
 * Deploy: put a certain number of army units on a target territory
 */
class DeployOrder final : public Order {
public:
    explicit DeployOrder(const GameEngine &gameEngine, Player &player, Territory &target, unsigned armies);
    DeployOrder(const DeployOrder &order);
    ~DeployOrder() override;
    [[nodiscard]] Order &clone() const override;
    [[nodiscard]] OrderType type() const override;
    [[nodiscard]] bool validate() override;
    void execute() override;
    DeployOrder &operator=(const DeployOrder&);
    std::ostream& printTo(std::ostream& os) const override;
    [[nodiscard]] unsigned armies() const;
private:
    Territory *targetTerritory_;  // weak ptr
    unsigned *armies_;
};


/**
 * Advance: move a certain number of army units from a source territory to a target territory
 */
class AdvanceOrder final : public Order {
public:
    explicit AdvanceOrder(const GameEngine &gameEngine, Player &player, Territory &source, Territory &target, unsigned armies);
    AdvanceOrder(const AdvanceOrder &order);
    ~AdvanceOrder() override;
    [[nodiscard]] Order& clone() const override;
    [[nodiscard]] OrderType type() const override;
    [[nodiscard]] bool validate() override;
    void execute() override;
    AdvanceOrder &operator=(const AdvanceOrder&);
    std::ostream& printTo(std::ostream& os) const override;
private:
    Territory *sourceTerritory_;  // weak ptr
    Territory *targetTerritory_;  // weak ptr
    unsigned *armies_;
};


/**
 * Bomb: destroy half of the army units located on a target territory. This order can only be issued if
 * a player has the bomb card in their hand.
 */
class BombOrder final : public Order {
public:
    explicit BombOrder(const GameEngine &gameEngine, const Player &player, const Territory &target);
    BombOrder(const BombOrder &order);
    ~BombOrder() override;
    [[nodiscard]] Order& clone() const override;
    [[nodiscard]] OrderType type() const override;
    [[nodiscard]] bool validate() override;
    void execute() override;
    BombOrder &operator=(const BombOrder&);
    std::ostream& printTo(std::ostream& os) const override;
private:
    const Territory *targetTerritory_; // weak ptr
};


/**
 * Blockade: Triple the number of army units on a target territory and make it a neutral territory. This
 * order can only be issued if a player has the blockade card in their hand.
 */
class BlockadeOrder final : public Order {
public:
    explicit BlockadeOrder(const GameEngine &gameEngine, const Player &player, Territory &target);
    BlockadeOrder(const BlockadeOrder &order);
    ~BlockadeOrder() override;
    [[nodiscard]] Order& clone() const override;
    [[nodiscard]] OrderType type() const override;
    [[nodiscard]] bool validate() override;
    void execute() override;
    BlockadeOrder &operator=(const BlockadeOrder&);
    std::ostream& printTo(std::ostream& os) const override;
private:
    Territory *targetTerritory_; // weak ptr
};

/**
 * Airlift: advance a certain number of army units from one from source territory to a target
 * territory. This order can only be issued if a player has the airlift card in their hand.
 */
class AirliftOrder final : public Order {
public:
    explicit AirliftOrder(const GameEngine &gameEngine, const Player &player, Territory &source, Territory &target, unsigned armies);
    AirliftOrder(const AirliftOrder &order);
    ~AirliftOrder() override;
    [[nodiscard]] Order& clone() const override;
    [[nodiscard]] OrderType type() const override;
    [[nodiscard]] bool validate() override;
    void execute() override;
    AirliftOrder &operator=(const AirliftOrder&);
    std::ostream& printTo(std::ostream& os) const override;
private:
    Territory *sourceTerritory_; // weak ptr
    Territory *targetTerritory_; // weak ptr
    unsigned *armies_;
};

/**
 * Negotiate: prevent attacks between the current player and another target player until the end
 * of the turn. This order can only be issued if a player has the diplomacy card in their hand.
 */
class NegotiateOrder final : public Order {
public:
    explicit NegotiateOrder(const GameEngine &gameEngine, const Player &player, Player &otherPlayer);
    NegotiateOrder(const NegotiateOrder &order);
    ~NegotiateOrder() override;
    [[nodiscard]] Order& clone() const override;
    [[nodiscard]] OrderType type() const override;
    [[nodiscard]] bool validate() override;
    void execute() override;
    NegotiateOrder &operator=(const NegotiateOrder&);
    std::ostream& printTo(std::ostream& os) const override;
private:
    Player *otherPlayer_; // weak ptr
};

/**
 * OrdersList class contains a list of Order objects
 */
class OrdersList final : public ILoggable, public Subject {
public:
    OrdersList();
    OrdersList(const OrdersList&);
    ~OrdersList() override;
    OrdersList& addOrder(const Order &order);
    OrdersList& remove(int index);
    OrdersList& move(int from, int to);
    OrdersList& executeOrders();
    [[nodiscard]] bool done() const;
    [[nodiscard]] const std::vector<Order*> &orders() const;
    [[nodiscard]] std::string stringToLog() const override;
    OrdersList &operator=(const OrdersList&);
    friend std::ostream& operator<<(std::ostream& os, const OrdersList& ordersList);
private:
    std::vector<Order*> *orders_; // Store pointers to Order objects
};

#endif
