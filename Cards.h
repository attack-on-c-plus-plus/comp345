#ifndef COMP345_CARDS_H
#define COMP345_CARDS_H

#include <string>
#include <vector>
#include <memory>

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

enum class CardType {
    bomb,
    reinforcement,
    blockade,
    airlift,
    diplomacy
};

/**
 * The Card Class.
 * Stores a pointer to a CardType.
 */
class Card {
public:
    explicit Card(const CardType& type);
    Card(const Card&);
    ~Card();
    [[nodiscard]] const CardType &type() const;
    [[nodiscard]] const Card &play() const;
    bool operator==(const Card& card) const;
    Card &operator=(const Card&);
    friend std::ostream &operator<<(std::ostream &os, const Card &card);
private:
    CardType *type_;
};

/**
 * The Hand Class.
 * Stores a pointer to a vector of Cards.
 */
class Hand {
public:
    explicit Hand(unsigned size = 0);
    explicit Hand(const std::vector<Card>& cardCollection);
    Hand(const Hand&);
    ~Hand();
    Hand &add(const Card *card);
    Hand &remove(const Card &card);
    Hand &discard(const Card &card, Deck& deck);
    [[nodiscard]] const Card &card(size_t index) const;
    [[nodiscard]] bool empty() const;
    [[nodiscard]] const std::vector<const Card*> &cards() const;
    [[nodiscard]] size_t size() const;
    Hand &operator=(const Hand&);
    friend std::ostream &operator<<(std::ostream &os, const Hand &hand);
private:
    std::vector<const Card*> *cards_;
};

/**
 * The Hand Class.
 * Stores a pointer to a vector of Cards.
 */
class Deck {
public:
    explicit Deck(unsigned size = 5);
    explicit Deck(const std::vector<Card> &cardDeck);
    Deck(const Deck&);
    ~Deck();
    Deck &add(const Card *card);
    Deck &remove(const Card &card);
    Deck &draw(Hand &hand);
    [[nodiscard]] bool empty() const;
    [[nodiscard]] size_t size() const;
    [[nodiscard]] const std::vector<const Card*> &cards() const;
    [[nodiscard]] const Card &card(size_t index) const;
    Deck &operator=(const Deck&);
    friend std::ostream &operator<<(std::ostream &os, const Deck &deck);
private:
    std::vector<const Card*> *cards_;
};

#endif