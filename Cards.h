#ifndef COMP345_CARDS_H
#define COMP345_CARDS_H

#include <string>
#include <vector>

// Daniel Soldera
// Carson Senthilkumar
// Joe El-Khoury
// Henri Stephane Carbon
// Haris Mahmood

/**
 * Cards.h includes groups of classes that implements a deck and a hand of Warzone cards.
 */

class Card;
class Deck;
class Hand;

/**
 * The Card Class.
 * Stores a pointer to a string.
 */
class Card {
    private:
        std::string *cardType;
    public:
        explicit Card(const std::string& cardType);
        Card(const Card&);
        ~Card();
        [[nodiscard]] std::string getCardType() const;
        void play();
        bool operator==(const Card& card) const;
        Card &operator=(const Card&);
//        friend std::istream &operator>>(std::istream &is, Card &card);
        friend std::ostream &operator<<(std::ostream &os, Card &card);
};

/**
 * The Hand Class.
 * Stores a pointer to a vector of Cards.
 */
class Hand {
    private:
        std::vector<Card> *cardCollection;
    public:
        explicit Hand(const int &collectionSize = 5);
        explicit Hand(const std::vector<Card>& cardCollection);
        Hand &addCard(const Card &card);
        Hand &removeCard(const unsigned &index);
        Hand(const Hand&);
        ~Hand();
        Card& getCard(unsigned index);
        [[nodiscard]] unsigned int getCardCount() const;
        Hand &operator=(const Hand&);
//        friend std::istream &operator>>(std::istream &is, Hand &hand);
        friend std::ostream &operator<<(std::ostream &os, Hand &hand);
};

/**
 * The Hand Class.
 * Stores a pointer to a vector of Cards.
 */
class Deck {
    private:
        std::vector<Card> *cardDeck;
    public:
        explicit Deck(const int &deckSize = 5);
        explicit Deck(const std::vector<Card> &cardDeck);
        Deck &addCard(const Card& card);
        Deck &removeCard(const unsigned &index);
        Deck(const Deck&);
        ~Deck();
        void draw(Hand&);
        [[nodiscard]] unsigned int getCardCount() const;
        Card& getCard(unsigned index);
        Deck &operator=(const Deck&);
//        friend std::istream &operator>>(std::istream &is, Deck &deck);
        friend std::ostream &operator<<(std::ostream &os, Deck &deck);
};

#endif