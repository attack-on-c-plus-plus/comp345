
#ifndef COMP345_PLAYER_H
#define COMP345_PLAYER_H

#include <string>
#include <vector>
#include <memory>
#include "Cards.h"
#include "Map.h"
#include "Orders.h"
#include "GameEngine.h"
#include "CommandProcessing.h"

// Daniel Soldera
// Carson Senthilkumar
// Joe El-Khoury
// Henri Stephane Carbon
// Haris Mahmood

// forward declaration
class Map;
class Territory;
class OrdersList;
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
    Player(const Player &other);
    //Destructor
    ~Player();
    //Methods
    [[nodiscard]] std::string getName() const;
    void changeName(const std::string &newName);
    [[nodiscard]] std::vector<const Territory *> toAttack() const;
    [[nodiscard]] std::vector<const Territory *> toDefend() const;
    void issueOrder();
    OrdersList &orderList();
    // Add methods to manage the player's territory
    void add(Territory &territory);
    [[nodiscard]] const std::vector<Territory *> &getTerritories() const;
    [[nodiscard]] const std::vector<Player *> &getCantAttack() const;
    void addNegotiator(Player negotiator) const;
    void removeNegotiators();
    // Add methods to manage the player's hand of cards
    void draw();
    void play(const Card &card, Territory &target);
    [[nodiscard]] const Hand &getHand() const;
private:
    void getReinforcements();
    std::string *name;
    std::vector<Territory *> *territories;
    OrdersList *ordersList;
    std::vector<Player *> *cantTarget;
    int *reinforcementPool;
    Hand *hand;
    GameEngine *gameEngine_;
    CommandProcessor *commandProcessor_;
};


#endif //COMP345_PLAYER_H