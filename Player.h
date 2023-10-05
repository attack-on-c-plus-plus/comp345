//
// Created by Haris Mahmood on 2023-09-27.
//

#ifndef COMP345_PLAYER_H
#define COMP345_PLAYER_H
#include <string>

//Temp will need to remove after
class Map{};
class Territory{};
class OrdersList{};
class Hand{};
class Deck{};
class Card{};
//------------------------

class Player {
    private:
        //Attributes
        std::string *name;
        std::vector<Territory> territories;
        OrdersList ordersList;
        Hand hand;

    public:

        //Constructor
        Player();
        Player(const std::string &name);
        Player(const Player& other);

        //Destructor
        ~Player();

        //Methods
        std::string getName() const;
        void changeName(const std::string &newName);

        std::vector<Territory> toAttack();
        std::vector<Territory> toDefend();

        void issueOrder(const std::string &orderType, int sourceTerritory, int targetTerritory, int armies, int targetPlayerID);

        // Add methods to manage the player's territory
        const std::vector<Territory>& getTerritoriesOwned() const;
        void addTerritory(const Territory &territory);
        bool removeTerritory(const Territory &territoryToRemove);

        // Add methods to manage the player's hand of cards
        void drawCardFromDeck(Deck& deck);
        void playCardFromHand(const Card& card, Deck& deck);
        const Hand& getHand() const;


};
#endif //COMP345_PLAYER_H


