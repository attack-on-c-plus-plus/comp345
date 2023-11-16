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
#include "PlayerStrategies.h"

// forward declaration
enum class Strategy;
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
    explicit Player(GameEngine &gameEngine, const std::string &name, Strategy strategy);
    Player(const Player &player);
    //Destructor
    ~Player();
    //Methods
    Player &operator=(const Player &player);
    bool operator==(const Player &player) const;
    [[nodiscard]] std::string name() const;
    void name(const std::string &newName);
    [[nodiscard]] bool isDeploying() const;
    [[nodiscard]] bool isIssuingOrders() const;
    [[nodiscard]] unsigned reinforcementPool() const;
    [[nodiscard]] Strategy strategy() const;
    [[nodiscard]] OrdersList &orderList() const;
    [[nodiscard]] const Hand &hand() const;
    [[nodiscard]] const std::vector<Territory *> &territories() const;
    [[nodiscard]] std::vector<const Territory *> toAttack() const;
    [[nodiscard]] std::vector<const Territory *> toDefend() const;
    [[nodiscard]] const std::vector<const Player *> &cantAttack() const;
    void issueOrders() const;
    void issueOrder() const;
    void add(Territory &territory) const;
    void addNegotiator(const Player &negotiator) const;
    void removeNegotiators() const;
    void draw() const;
    void play(const Card &card, Territory &target);
    void fillReinforcementPool() const;
    void deploy(unsigned armies) const;
    void doneOrders() const;
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
    Strategy* strategy_;
    PlayerStrategy* playerStrategy_;
    [[nodiscard]] unsigned int continentBonusArmies() const;
    [[nodiscard]] unsigned int territoryBonusArmies() const;
    PlayerStrategy* createStrategy(Strategy strategy);
    friend std::ostream &operator<<(std::ostream &os, const Player &player);
};


#endif //COMP345_PLAYER_H