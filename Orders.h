// Orders.h
#ifndef COMP345_ORDERS_H
#define COMP345_ORDERS_H
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

class Order {
public:
    virtual ~Order() {}
    virtual bool validate() const = 0;
    virtual void execute() = 0;
    friend std::ostream& operator<<(std::ostream& os, const Order& order);
    void swap(Order& other) {
        std::swap(*description, *(other.description));
        std::swap(*effect, *(other.effect));
    }
// Public member functions to access and modify description and effect
    const std::string& getDescription() const {
        return *description;
    }

    const std::string& getEffect() const {
        return *effect;
    }

    void setDescription(const std::string& newDescription) {
        *description = newDescription;
    }

    void setEffect(const std::string& newEffect) {
        *effect = newEffect;
    }

protected:
    std::string *description;
    std::string *effect;
};


// List of all order types, each one implemented as a subclass of Order.
// To do: add the necessary parameters for each order type


//Deploy: put a certain number of army units on a target territory

class DeployOrder : public Order {
public:
    DeployOrder(int armies);
    bool validate() const override;
    void execute() override;

private:
    int *armies_to_deploy;

};


//Advance: move a certain number of army units from one territory (source territory) to another territory (target 
//territory)

class AdvanceOrder : public Order {
public:
    AdvanceOrder(int source, int target, int armies);
    bool validate() const override;
    void execute() override;

private:
    int *source_territory;
    int *target_territory;
    int *armies_to_advance;

};


//Bomb: destroy half of the army units located on a target territory. This order can only be issued if a player 
//has the bomb card in their hand. 

class BombOrder : public Order {
public:
    BombOrder(int target);
    bool validate() const override;
    void execute() override;

private:
    int *target_territory;

};


//Blockade: Triple the number of army units on a target territory and make it a neutral territory. This order can 
//only be issued if a player has the blockade card in their hand.

class BlockadeOrder : public Order {
public:
    BlockadeOrder(int target);
    bool validate() const override;
    void execute() override;

private:
    int *target_territory;

};


//Airlift: advance a certain number of army units from one from one territory (source territory) to another 
//territory (target territory). This order can only be issued if a player has the airlift card in their hand

class AirliftOrder : public Order {
public:
    AirliftOrder(int armies, int source, int target);
    bool validate() const override;
    void execute() override;

private:
    int *source_territory;
    int *target_territory;
    int *armies_to_advance;
};


//Negotiate: prevent attacks between the current player and another target player until the end of the turn. This 
//order can only be issued if a player has the diplomacy card in their hand.

class NegotiateOrder : public Order {
public:
    NegotiateOrder(int targetPlayer);
    bool validate() const override;
    void execute() override;

private:
    int *target_player;   
};

//OrdersList class contains a list of Order objects 

class OrdersList {
    OrdersList &addOrder(const Order &order);
    OrdersList &removeOrder(int index);
    OrdersList &moveOrder(int from, int to);
    OrdersList &executeOrders();

private:
    std::vector<Order> *orders;
};
#endif