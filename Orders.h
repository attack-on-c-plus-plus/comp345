// Orders.h
#ifndef COMP345_ORDERS_H
#define COMP345_ORDERS_H

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include "Map.h"

// Daniel Soldera
// Carson Senthilkumar
// Joe El-Khoury
// Henri Stephane Carbon
// Haris Mahmood

class Order {
public:
    Order(const std::string &description);
    // Copy constructor
    Order(const Order&);
    virtual ~Order();
    [[nodiscard]] virtual Order* clone() const = 0;
    [[nodiscard]] virtual bool validate() const = 0;
    virtual void execute() = 0;
    friend std::ostream& operator<<(std::ostream& os, const Order& order);
    // Public member functions to access and modify description and effect
    [[nodiscard]] const std::string &description() const;
    [[nodiscard]] const std::string &effect() const;
    Order &operator=(const Order&);

protected:
    std::string *description_;
    std::string *effect_;
};


// List of all order types, each one implemented as a subclass of Order.

//Deploy: put a certain number of army units on a target territory

class DeployOrder : public Order {
public:
    explicit DeployOrder(unsigned armies);
    DeployOrder(const DeployOrder&);
    ~DeployOrder() override;
    [[nodiscard]] DeployOrder* clone() const override;
    [[nodiscard]] bool validate() const override;
    void execute() override;
    DeployOrder &operator=(const DeployOrder&);
private:
    unsigned *armies_;
};


//Advance: move a certain number of army units from one territory (source territory) to another territory (target
//territory)

class AdvanceOrder : public Order {
public:
    explicit AdvanceOrder(size_t source, size_t target, unsigned armies);
    AdvanceOrder(const AdvanceOrder&);
    ~AdvanceOrder() override;
    [[nodiscard]] AdvanceOrder* clone() const override;
    [[nodiscard]] bool validate() const override;
    void execute() override;
    AdvanceOrder &operator=(const AdvanceOrder&);
private:
    size_t *sourceTerritory_;
    size_t *targetTerritory_;
    unsigned *armies_;
};


//Bomb: destroy half of the army units located on a target territory. This order can only be issued if a player
//has the bomb card in their hand.

class BombOrder : public Order {
public:
    explicit BombOrder(size_t target);
    BombOrder(const BombOrder&);
    ~BombOrder() override;
    [[nodiscard]] BombOrder* clone() const override;
    [[nodiscard]] bool validate() const override;
    void execute() override;
    BombOrder &operator=(const BombOrder&);
private:
    size_t *targetTerritory_;
};


//Blockade: Triple the number of army units on a target territory and make it a neutral territory. This order can
//only be issued if a player has the blockade card in their hand.

class BlockadeOrder : public Order {
public:
    explicit BlockadeOrder(size_t target);
    BlockadeOrder(const BlockadeOrder&);
    ~BlockadeOrder() override;
    [[nodiscard]] BlockadeOrder* clone() const override;
    [[nodiscard]] bool validate() const override;
    void execute() override;
    BlockadeOrder &operator=(const BlockadeOrder&);
private:
    size_t *targetTerritory_;
};


//Airlift: advance a certain number of army units from one from one territory (source territory) to another
//territory (target territory). This order can only be issued if a player has the airlift card in their hand

class AirliftOrder : public Order {
public:
    explicit AirliftOrder(size_t source, size_t target, unsigned armies);
    AirliftOrder(const AirliftOrder&);
    ~AirliftOrder() override;
    [[nodiscard]] AirliftOrder* clone() const override;
    [[nodiscard]] bool validate() const override;
    void execute() override;
    AirliftOrder &operator=(const AirliftOrder&);
private:
    size_t *sourceTerritory_;
    size_t *targetTerritory_;
    unsigned *armies_;
};


//Negotiate: prevent attacks between the current player and another target player until the end of the turn. This
//order can only be issued if a player has the diplomacy card in their hand.

class NegotiateOrder : public Order {
public:
    explicit NegotiateOrder(unsigned targetPlayer);
    NegotiateOrder(const NegotiateOrder&);
    ~NegotiateOrder() override;
    [[nodiscard]] NegotiateOrder* clone() const override;
    [[nodiscard]] bool validate() const override;
    void execute() override;
    NegotiateOrder &operator=(const NegotiateOrder&);
private:
    unsigned *targetPlayer_;
};

//OrdersList class contains a list of Order objects

class OrdersList {
public:
    OrdersList();
    OrdersList(const OrdersList&);
    ~OrdersList();

    OrdersList& addOrder(Order* order);
    OrdersList& remove(int index);
    OrdersList& move(int from, int to);
    OrdersList& executeOrders();
    [[nodiscard]]const std::vector<Order*> &getOrder() const;
    OrdersList &operator=(const OrdersList&);
private:
    std::vector<Order*> *orders_; // Store pointers to Order objects
};


#endif
