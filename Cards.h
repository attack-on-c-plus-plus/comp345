#ifndef COMP345_CARDS_H
#define COMP345_CARDS_H

#include <string>
#include <vector>

/**
 * Cards.h includes groups of classes that implements a deck and a hand of Warzone cards.
*/

class Card;
class Deck;
class Hand;

class Card {
    private:
        std::string *cardType;
    public:
        explicit Card(const std::string& cardType);
        // Copy Constructor
        Card(const Card&);
        // Destructor
        ~Card();
        
        [[nodiscard]] std::string getCardType() const;

        // play() method
        void play();
};

class Hand {
    private:
        std::vector<Card> *cardCollection;
    public:
        explicit Hand();
        explicit Hand(const std::vector<Card>& cardCollection);
        // Copy Constructor
        Hand(const Hand&);
        // Destructor
        ~Hand();
};

class Deck {
    private:
        std::vector<Card> *cardDeck;
    public:
        explicit Deck();
        explicit Deck(const std::vector<Card> &cardDeck);
        Deck(const Deck&);
        ~Deck();
        // To implement when Player is completed
        void draw();

};

#endif