// Orders.h
#pragma once
#include <vector>
#include <string>

class Order {
public:
    virtual ~Order() {}
    virtual bool validate() const = 0;
    virtual void execute() = 0;

protected:
    std::string description;
    std::string effect;
};


// List of all order types, each one implemented as a subclass of Order.
// To do: add the necessary parameters for each order type


//Deploy
class DeployOrder : public Order {
public:
    DeployOrder();
    bool validate() const override;
    void execute() override;

};


//Advance
class AdvanceOrder : public Order {
public:
    AdvanceOrder();
    bool validate() const override;
    void execute() override;

};


//Bomb
class BombOrder : public Order {
public:
    BombOrder();
    bool validate() const override;
    void execute() override;

};


//Blockade
class BlockadeOrder : public Order {
public:
    BlockadeOrder();
    bool validate() const override;
    void execute() override;

};


//Airlift
class AirliftOrder : public Order {
public:
    AirliftOrder();
    bool validate() const override;
    void execute() override;

};


//Negotiate
class NegotiateOrder : public Order {
public:
    NegotiateOrder();
    bool validate() const override;
    void execute() override;
   
};

//OrdersList class contains a list of Order objects 

class OrdersList {
public:
    void addOrder(Order* order);
    void remove(int index);
    void move(int from, int to);
    void executeOrders();

private:
    std::vector<Order*> orders;
};
