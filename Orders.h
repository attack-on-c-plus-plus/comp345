// Orders.h
#ifndef COMP345_ORDERS_H
#define COMP345_ORDERS_H

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

class Order {
public:
    Order();
    // Copy constructor
    Order(const Order&);
    virtual ~Order();
    [[nodiscard]] virtual Order* clone() const = 0;
    [[nodiscard]] virtual bool validate() const = 0;
    virtual void execute() = 0;
    friend std::ostream& operator<<(std::ostream& os, const Order& order);
    void swap(Order& other) {
        std::swap(*description, *(other.description));
        std::swap(*effect, *(other.effect));
    }
    // Public member functions to access and modify description and effect
    [[nodiscard]] const std::string &getDescription() const;
    [[nodiscard]] const std::string& getEffect() const;
    void setDescription(const std::string& newDescription);
    void setEffect(const std::string& newEffect);
    Order &operator=(const Order&);

protected:
    std::string *description;
    std::string *effect;
};


// List of all order types, each one implemented as a subclass of Order.
// To do: add the necessary parameters for each order type


//Deploy: put a certain number of army units on a target territory

class DeployOrder : public Order {
public:
    explicit DeployOrder(int armies);
    DeployOrder(const DeployOrder&);
    ~DeployOrder() override;
    [[nodiscard]] DeployOrder* clone() const override;
    [[nodiscard]] bool validate() const override;
    void execute() override;
    DeployOrder &operator=(const DeployOrder&);
private:
    int *armies_to_deploy;
};


//Advance: move a certain number of army units from one territory (source territory) to another territory (target
//territory)

class AdvanceOrder : public Order {
public:
    AdvanceOrder(int source, int target, int armies);
    AdvanceOrder(const AdvanceOrder&);
    ~AdvanceOrder() override;
    [[nodiscard]] AdvanceOrder* clone() const override;
    [[nodiscard]] bool validate() const override;
    void execute() override;
    AdvanceOrder &operator=(const AdvanceOrder&);
private:
    int *source_territory;
    int *target_territory;
    int *armies_to_advance;
};


//Bomb: destroy half of the army units located on a target territory. This order can only be issued if a player
//has the bomb card in their hand.

class BombOrder : public Order {
public:
    explicit BombOrder(int target);
    BombOrder(const BombOrder&);
    ~BombOrder() override;
    [[nodiscard]] BombOrder* clone() const override;
    [[nodiscard]] bool validate() const override;
    void execute() override;
    BombOrder &operator=(const BombOrder&);
private:
    int *target_territory;
};


//Blockade: Triple the number of army units on a target territory and make it a neutral territory. This order can
//only be issued if a player has the blockade card in their hand.

class BlockadeOrder : public Order {
public:
    explicit BlockadeOrder(int target);
    BlockadeOrder(const BlockadeOrder&);
    ~BlockadeOrder() override;
    [[nodiscard]] BlockadeOrder* clone() const override;
    [[nodiscard]] bool validate() const override;
    void execute() override;
    BlockadeOrder &operator=(const BlockadeOrder&);
private:
    int *target_territory;
};


//Airlift: advance a certain number of army units from one from one territory (source territory) to another
//territory (target territory). This order can only be issued if a player has the airlift card in their hand

class AirliftOrder : public Order {
public:
    AirliftOrder(int armies, int source, int target);
    AirliftOrder(const AirliftOrder&);
    ~AirliftOrder() override;
    [[nodiscard]] AirliftOrder* clone() const override;
    [[nodiscard]] bool validate() const override;
    void execute() override;
    AirliftOrder &operator=(const AirliftOrder&);
private:
    int *source_territory;
    int *target_territory;
    int *armies_to_advance;
};


//Negotiate: prevent attacks between the current player and another target player until the end of the turn. This
//order can only be issued if a player has the diplomacy card in their hand.

class NegotiateOrder : public Order {
public:
    explicit NegotiateOrder(int targetPlayer);
    NegotiateOrder(const NegotiateOrder&);
    ~NegotiateOrder() override;
    [[nodiscard]] NegotiateOrder* clone() const override;
    [[nodiscard]] bool validate() const override;
    void execute() override;
    NegotiateOrder &operator=(const NegotiateOrder&);
private:
    int *target_player;
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
    [[nodiscard]] std::vector<Order*> &getOrder() const;
    OrdersList &operator=(const OrdersList&);
private:
    std::vector<Order*> *orders; // Store pointers to Order objects
};


#endif