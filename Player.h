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

#include <set>
#include <string>
#include <vector>

class PlayerStrategy;
enum class CardType;
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
    [[nodiscard]] unsigned reinforcementPool() const;
    [[nodiscard]] Strategy strategy() const;
    [[nodiscard]] OrdersList &orderList() const;
    [[nodiscard]] const Hand &hand() const;
    [[nodiscard]] const GameEngine &gameEngine() const;
    [[nodiscard]] const std::vector<Territory *> &territories() const;
    [[nodiscard]] std::vector<const Territory *> toAttack() const;
    [[nodiscard]] std::vector<Territory *> toDefend() const;
    [[nodiscard]] const std::vector<const Player *> &cantAttack() const;
    void issueOrders() const;
    void issueOrder() const;
    void add(Territory &territory) const;
    void remove(const Territory& territory) const;
    void addNegotiator(const Player &negotiator) const;
    void removeNegotiators() const;
    void draw() const;
    void play(CardType cardType, std::istream &is) const;
    void fillReinforcementPool() const;
    void deploy(unsigned armies) const;
    [[nodiscard]] unsigned availableReinforcements() const;
    void attacked();
    [[nodiscard]] bool receivedCard() const;
    void receivedCard(bool received) const;
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
    bool *receivedCard_;
    [[nodiscard]] unsigned int continentBonusArmies() const;
    [[nodiscard]] unsigned int territoryBonusArmies() const;
    PlayerStrategy* createStrategy(Strategy strategy);
    friend std::ostream &operator<<(std::ostream &os, const Player &player);
};

class Players {
public:
    explicit Players(GameEngine &gameEngine);
    Players(const Players &players) = delete;
    ~Players();
    Players &operator=(const Players &players) = delete;
    void setPlayOrder() const;
    [[nodiscard]] bool add(const Player&player) const;
    [[nodiscard]] Player& neutral() const;
    [[nodiscard]] bool has(const Player& player) const;
    [[nodiscard]] Player& player(unsigned id) const;
    void issueOrders() const;
    void executeOrders() const;
    void reinforcement() const;
    [[nodiscard]] bool hasMinimum() const;
    [[nodiscard]] bool hasMaximum() const;
    void init(std::ostream&os) const;
    [[nodiscard]] size_t size() const;
    [[nodiscard]] std::string winner() const;

private:
    GameEngine *gameEngine_;
    unsigned *currentTurn_;
    std::vector<Player *> *players_;
    friend std::ostream &operator<<(std::ostream &os, const Players &players);
};

#endif //COMP345_PLAYER_H