
#ifndef COMP345_PLAYER_H
#define COMP345_PLAYER_H

#include <string>
#include <vector>
#include <memory>
#include "Cards.h"
#include "Map.h"
#include "Orders.h"

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

class Player {
public:
    //Constructor
    Player();

    explicit Player(const std::string &name);

    Player(const Player &other);

    //Destructor
    ~Player();

    //Methods
    [[nodiscard]] std::string &getName() const;
    void changeName(const std::string &newName);

    [[nodiscard]] std::vector<const Territory *> toAttack(const Map &) const;
    [[nodiscard]] std::vector<const Territory *> toDefend(const Map &) const;

    void issueOrder(const std::string &orderType, Territory &source, Territory &target, unsigned armies, const Player &otherPlayer, GameEngine &gameEngine);

    OrdersList &orderList();

    // Add methods to manage the player's territory
    void addTerritory(Territory &territory);

    [[nodiscard]] const std::vector<Territory *> &getTerritories() const;
    [[nodiscard]] const std::vector<Player *> &getCantAttack() const;
    [[nodiscard]] int getReinforcmentNumber() const;
    void addReinforcements(int armies);
    void addNegotiator(Player negotiator) const;
    void removeNegotiators();

    // Add methods to manage the player's hand of cards
    void drawCardFromDeck(Deck &deck);


    void playCardFromHand(const Card &card, Deck &deck, GameEngine &engine, Territory &target, Territory &source, Territory &territory, unsigned int armies);

    [[nodiscard]] const Hand &getHand() const;

private:
    //Attributes
    std::string *name;
    std::vector<Territory *> *territories;
    OrdersList *ordersList;
    std::vector<Player *> *cantTarget;
    int *reinforcementPool;
    Hand *hand;


};


#endif //COMP345_PLAYER_H
