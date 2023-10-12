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
 * Stores a pointer to a string.
 */
class Card {
public:
    explicit Card(const CardType& type);
    Card(const Card&);
    ~Card();
    [[nodiscard]] const CardType &type() const;
    void play() const;
    bool operator==(const Card& card) const;
    Card &operator=(const Card&);
    friend std::ostream &operator<<(std::ostream &os, const Card &card);
private:
    std::unique_ptr<CardType> type_;
};

/**
 * The Hand Class.
 * Stores a pointer to a vector of Cards.
 */
class Hand {
public:
    explicit Hand(const int &collectionSize = 5);
    explicit Hand(const std::vector<Card>& cardCollection);
    Hand &add(const std::shared_ptr<Card> &card);
    Hand &remove(const std::shared_ptr<Card> &card);
    Hand(const Hand&);
    ~Hand();
    [[nodiscard]] const std::shared_ptr<Card> &card(size_t index) const;
    [[nodiscard]] bool isEmpty() const;
    [[nodiscard]] const std::vector<std::shared_ptr<Card>> &cards() const;
    [[nodiscard]] size_t size() const;
    Hand &operator=(const Hand&);
    friend std::ostream &operator<<(std::ostream &os, const Hand &hand);
private:
    std::unique_ptr<std::vector<std::shared_ptr<Card>>> cards_;
};

/**
 * The Hand Class.
 * Stores a pointer to a vector of Cards.
 */
class Deck {
public:
    explicit Deck(const int &deckSize = 5);
    explicit Deck(const std::vector<Card> &cardDeck);
    Deck &add(const std::shared_ptr<Card> &card);
    Deck &remove(const std::shared_ptr<Card> &card);
    Deck(const Deck&);
    ~Deck();
    Deck &draw(Hand &hand);
    [[nodiscard]] bool isEmpty() const;
    [[nodiscard]] size_t size() const;
    [[nodiscard]] const std::vector<std::shared_ptr<Card>> &cards() const;
    [[nodiscard]] const std::shared_ptr<Card> &card(size_t index) const;
    Deck &operator=(const Deck&);
    friend std::ostream &operator<<(std::ostream &os, const Deck &deck);
private:
    std::unique_ptr<std::vector<std::shared_ptr<Card>>> cards_;
};

#endif