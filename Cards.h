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
        void play(Hand& hand, Deck& deck) const;
        bool operator==(const Card& card);
};

class Hand {
    private:
        std::vector<Card> *cardCollection;
    public:
        explicit Hand(const int &collectionSize = 5);
        explicit Hand(const std::vector<Card>& cardCollection);
        Hand &addCard(const Card &card);
        Hand &removeCard(const Card &card);
        // Copy Constructor
        Hand(const Hand&);
        // Destructor
        ~Hand();
        Card& getCard(unsigned index);
        unsigned int getCardCount();
};

class Deck {
    private:
        std::vector<Card> *cardDeck;
    public:
        explicit Deck(const int &deckSize = 5);
        explicit Deck(const std::vector<Card> &cardDeck);
        Deck &addCard(const Card& card);
        Deck &removeCard(const Card &card);
        Deck(const Deck&);
        ~Deck();
        // To implement when Player is completed
        void draw(Hand&);
        unsigned int getCardCount();
        Card& getCard(unsigned index);
};

#endif