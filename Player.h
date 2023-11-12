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

#ifndef COMP345_PLAYER_H
#define COMP345_PLAYER_H

#include <string>
#include <vector>

#include "Orders.h"

// forward declaration
class Map;
class Territory;
class OrdersList;
class GameEngine;
class Order;
class Deck;
class Card;
class Hand;
class CommandProcessor;

class Player {
public:
    //Constructor
    explicit Player(GameEngine &gameEngine, const std::string &name);
    explicit Player(GameEngine &gameEngine, const std::string &name, CommandProcessor &commandProcessor);
    Player(const Player &player);
    //Destructor
    ~Player();
    //Methods
    [[nodiscard]] std::string getName() const;
    void changeName(const std::string &newName);
    [[nodiscard]] std::vector<const Territory *> toAttack() const;
    [[nodiscard]] bool isDeploying() const;
    [[nodiscard]] bool isIssuingOrders() const;
    [[nodiscard]] std::vector<const Territory *> toDefend() const;
    void issueOrders();
    void issueOrder();
    [[nodiscard]] OrdersList &orderList() const;
    // Add methods to manage the player's territory
    void add(Territory &territory) const;
    [[nodiscard]] const std::vector<Territory *> &getTerritories() const;
    [[nodiscard]] const std::vector<const Player *> &getCantAttack() const;
    void addNegotiator(const Player &negotiator) const;
    void removeNegotiators() const;
    // Add methods to manage the player's hand of cards
    void draw() const;
    void play(const Card &card, Territory &target);
    [[nodiscard]] const Hand &getHand() const;
    [[nodiscard]] const unsigned reinforcementPool() const;
    Player &operator=(const Player &player);
    bool operator==(const Player &player) const;
    void fillReinforcementPool() const;
    void deploy(unsigned armies) const;
private:
    std::string *name_;
    std::vector<Territory *> *territories_;
    OrdersList *ordersList_;
    std::vector<const Player *> *cantTarget_;
    unsigned *reinforcementPool_;
    bool *deployComplete_;
    bool *ordersComplete_;
    Hand *hand_;
    GameEngine *gameEngine_;
    CommandProcessor *commandProcessor_;
    [[nodiscard]] unsigned int continentBonusArmies() const;
    [[nodiscard]] unsigned int territoryBonusArmies() const;
    friend std::ostream &operator<<(std::ostream &os, const Player &player);
};


#endif //COMP345_PLAYER_H