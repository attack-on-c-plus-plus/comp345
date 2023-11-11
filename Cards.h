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


#ifndef COMP345_CARDS_H
#define COMP345_CARDS_H

#include <memory>
#include <vector>

/**
 * Cards.h includes groups of classes that implements a deck and a hand of Warzone cards.
 */

class Player;
class Territory;
class GameEngine;
class BombOrder;
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
    explicit Card(CardType type);
    Card(const Card&);
    ~Card();
    [[nodiscard]] CardType type() const;
    [[nodiscard]] const Card &play(Player& player, Territory& territory, GameEngine &gameEngine) const;
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
class Deck {
public:
    explicit Deck(unsigned size = 5);
    explicit Deck(const std::vector<Card> &cardDeck);
    Deck(const Deck&);
    ~Deck();
    Deck &add(CardType cardType);
    Deck &remove(const Card &card);
    Deck &draw(const Hand &hand);
    Deck &discard(const Card &card, const Hand& fromHand);
    [[nodiscard]] bool empty() const;
    [[nodiscard]] size_t size() const;
    [[nodiscard]] const std::vector<const Card*> &cards() const;
    [[nodiscard]] const Card &card(size_t index) const;
    Deck &operator=(const Deck&);
private:
    std::vector<const Card*> *cards_;
    friend std::ostream &operator<<(std::ostream &os, const Deck &deck);
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
    Hand &add(const Card &card);
    Hand &remove(const Card &card);
    [[nodiscard]] const Card &card(size_t index) const;
    [[nodiscard]] bool empty() const;
    [[nodiscard]] const std::vector<const Card*> &cards() const;
    [[nodiscard]] size_t size() const;
    Hand &operator=(const Hand&);
private:
    std::vector<const Card*> *cards_;
    friend Deck &Deck::draw(const Hand &hand);
    friend Deck &Deck::discard(const Card &card, const Hand& fromHand);
    friend std::ostream &operator<<(std::ostream &os, const Hand &hand);
};

#endif
