#include "Cards.h"
#include <iostream>
#include <stdexcept>
#include <random>

// Daniel Soldera
// Carson Senthilkumar
// Joe El-Khoury
// Henri Stephane Carbon
// Haris Mahmood

/**
 * Creates a new card
 * @param type The card type
 */
Card::Card(const CardType &type) : type_(new CardType(type)) {}

/**
 * Creates a new Card by copying the card type of the card to be copied.
 * @param cardToCopy The card to be copied.
 */
Card::Card(const Card &cardToCopy) : type_{new CardType(*cardToCopy.type_)} {}

/**
 * Default Card destructor. Deletes the cardType pointer.
 */
Card::~Card() {
    delete type_;
}

/**
 * Gets the card type
 * @return A string that represents the card type.
 */
const CardType &Card::type() const {
    return *type_;
}

/**
 * Creates a special order
 */
const Card &Card::play(Player& player, Territory& territory) const {
    // Check the card type. For each case, play an order
    switch(*this->type_) {
        case CardType::bomb: {
            auto *order = new BombOrder(player, territory);
            order->execute();
            delete order;
        }
            break;
        case CardType::reinforcement: {}
            break;
        case CardType::blockade: {
            auto *order = new BlockadeOrder(player, territory);
            order->execute();
            delete order;
        }
            break;
        case CardType::airlift: {}
            break;
        case CardType::diplomacy: {}
            break;
    }
}

/**
 * An equals operator that compares the cardType
 * Required to find the card from a vector of cards
 * @param card The card to compare
 * @return True if the card type is the same, and false otherwise.
 */
bool Card::operator==(const Card &card) const {
    return (*this->type_ == *card.type_);
}

/**
 * Assignment operator for the Card class.
 * card1 = card2
 * If the address is not the same for both, then replace
 * the card type of card1 with that of card2.
 * @param card A card object to compare
 * @return A Card object
 */
Card &Card::operator=(const Card &card) {
    if (this != &card) {
        delete type_;
        type_ = new CardType(*card.type_);
    }
    return *this;
}

/**
 * Output stream operator of Card
 * @param os The output stream
 * @param card A card
 * @return the output stream itself
 */
std::ostream &operator<<(std::ostream &os, const Card &card) {
    switch (*card.type_) {
        case CardType::bomb:
            os << "bomb";
            break;
        case CardType::reinforcement:
            os << "reinforcement";
            break;
        case CardType::blockade:
            os << "blockade";
            break;
        case CardType::airlift:
            os << "airlift";
            break;
        case CardType::diplomacy:
            os << "diplomacy";
            break;
        default:
            os.setstate(std::ios_base::failbit);
    }
    return os;
}

/**
 * Deck constructor that initializes a deck of a specified size.
 * @param size The deck size. By default, the size is set to 5.
 */
Deck::Deck(unsigned int size) {
    cards_ = new std::vector<const Card *>();
    cards_->reserve(size);
}

/**
 * A constructor for the Deck class
 * @param cardDeck an existing vector of Cards
 */
Deck::Deck(const std::vector<Card> &cardDeck) {
    cards_ = new std::vector<const Card *>();
    for (const auto &c: cardDeck) {
        cards_->push_back(new Card(c));
    }
}

/**
 * Copy constructor for Deck
 * @param deckToCopy The deck to be copied.
 */
Deck::Deck(const Deck &deckToCopy) {
    cards_ = new std::vector<const Card *>();
    for (const auto &c: *deckToCopy.cards_) {
        cards_->push_back(new Card(*c));
    }
}

/**
 * Destructor for the Deck class. Deletes the cardDeck pointer.
 */
Deck::~Deck() {
    for (auto c: *cards_) delete c;
    cards_->clear();
    delete cards_;
};

/**
 * Adds a card to the Deck.
 * @param card The card to be added.
 * @return The Deck with the new card.
 */
Deck &Deck::add(const Card &card) {
    cards_->push_back(new Card(card));
    return *this;
}

/**
 * Removes a card from the Deck and destroys it.
 * @param card The card to be removed
 * @return The deck with the removed card.
 */
Deck &Deck::remove(const Card &card) {
    for (auto it = cards_->begin(); it != cards_->end(); ++it) {
        if (**it == card) {
            delete *it;
            cards_->erase(it);
            return *this;
        }
    }
    return *this;
}

/**
 * Draw cards from the deck and place it directly into a hand
 * @param hand The hand where the randomly chosen card will be placed.
 */
Deck &Deck::draw(Hand &hand) {
    if (cards_->empty()) {
        throw std::out_of_range("No more cards in the deck");
    }
    std::random_device r;
    std::uniform_int_distribution<size_t> u(0, cards_->size() - 1);
    std::default_random_engine e(r());
    auto random = u(e);

    hand.cards_->push_back(cards_->at(random));
    cards_->erase(cards_->begin() + random);

    return *this;
}

/**
 * Discards a card back from the supplied hand back to this deck
 * @param card
 * @param deck
 * @return The hand
 */
Deck &Deck::discard(const Card &card, Hand &fromHand) {
    for (auto it = fromHand.cards_->begin(); it != fromHand.cards_->end(); ++it) {
        if (**it == card) {
            cards_->push_back(*it);
            fromHand.cards_->erase(it);
            return *this;
        }
    }
    return *this;
}

/**
 * Gets a card from a deck
 * @param index The index of the selected card from the Deck of cards.
 * @return The selected card
 */
const Card &Deck::card(size_t index) const {
    return *cards_->at(index);
}

/**
 * Gets the number of cards in the Deck
 * @return an int indicating the number of cards
 */
size_t Deck::size() const {
    return cards_->size();
}

/**
 * Gets if a deck is empty
 * @return true if empty; false otherwise
 */
bool Deck::empty() const {
    return cards_->empty();
}

/**
 * Gets the underlying cards in the deck
 * @return
 */
const std::vector<const Card *> &Deck::cards() const {
    return *cards_;
}

/**
 * Assignment operator for the Deck class.
 * deck1 = deck2
 * If the address is not the same for both, then replace
 * the cardDeck of deck1 with that of deck2.
 * @param deck A deck of cards
 * @return A Deck object
 */
Deck &Deck::operator=(const Deck &deck) {
    if (this != &deck) {
        for (auto c: *cards_) delete c;
        cards_->clear();
        delete cards_;
        cards_ = new std::vector<const Card *>();
        for (const auto &c: *deck.cards_) {
            add(*c);
        }
    }
    return *this;
}

/**
 * Output stream when printing deck to console
 * @param os The output stream
 * @param deck The hand object
 * @return the output stream
 */
std::ostream &operator<<(std::ostream &os, const Deck &deck) {
    os << "Deck (size: " << deck.cards_->size() << "): " << std::endl;
    for (auto &card: *deck.cards_) {
        os << "\t" << *card << std::endl;
    }
    return os;
}

/**
 * Creates a hand object of a specific size
 * @param size: The number of cards a hand can hold initially.
 */
Hand::Hand(unsigned int size) :
        cards_{new std::vector<const Card *>()} {
    cards_->reserve(size);
}

/**
 * Creates a hand object by providing an existing card collection
 * @param cardCollection The card collection.
 */
Hand::Hand(const std::vector<Card> &cardCollection) {
    cards_ = new std::vector<const Card *>();
    for (const auto &c: cardCollection) {
        cards_->push_back(new Card(c));
    }
}

/**
 * Creates a hand by copying another hand.
 * @param handToCopy The hand to be copied.
 */
Hand::Hand(const Hand &handToCopy) {
    cards_ = new std::vector<const Card *>();
    for (const auto &c: *handToCopy.cards_) {
        cards_->push_back(new Card(*c));
    }
}

/**
 * Default Card destructor. Deletes the cardType pointer.
 */
Hand::~Hand() {
    for (auto c: *cards_) delete c;
    cards_->clear();
    delete cards_;
    cards_ = new std::vector<const Card *>();
    delete cards_;
};

/**
 * Adds a card to the hand.
 * @param card The card to be added (hand becomes owner of the pointer).
 * @return The hand with the new card.
 */
Hand &Hand::add(const Card &card) {
    cards_->push_back(new Card(card));
    return *this;
}

/**
 * Removes the card from the hand and destroys it.
 * @param card
 * @return The hand
 */
Hand &Hand::remove(const Card &card) {
    for (auto it = cards_->begin(); it != cards_->end(); ++it) {
        if (**it == card) {
            delete *it;
            cards_->erase(it);
            return *this;
        }
    }
    return *this;
}

/**
 * Gets a card from the player's hand
 * @param index The index of the selected card from the player's card collection.
 * @return The selected card
 */
const Card &Hand::card(size_t index) const {
    return *cards_->at(index);
}

/**
 * Gets the number of cards in the player's hand
 * @return an int indicating the size of the player's hand
 */
size_t Hand::size() const {
    return cards_->size();
}

/**
 * Gets if a deck is empty
 * @return true if empty; false otherwise
 */
bool Hand::empty() const {
    return cards_->empty();
}

/**
 * Gets the underlying cards in the deck
 * @return
 */
const std::vector<const Card *> &Hand::cards() const {
    return *cards_;
}

/**
 * Assignment operator for the hand class.
 * hand1 = hand2
 * If the address is not the same for both, then replace
 * the cardCollection of hand1 with that of hand2.
 * @param hand A hand object to compare
 * @return A hand object
 */
Hand &Hand::operator=(const Hand &hand) {
    if (this != &hand) {
        for (auto c: *cards_) delete c;
        cards_->clear();
        delete cards_;
        cards_ = new std::vector<const Card *>(hand.cards_->size());
        for (const auto &c: *hand.cards_) {
            add(*c);
        }
    }
    return *this;
}

/**
 * Output stream when printing hand to console
 * @param os The output stream
 * @param hand The hand object
 * @return the output stream
 */
std::ostream &operator<<(std::ostream &os, const Hand &hand) {
    os << "Hand (size: " << hand.cards_->size() << "): " << std::endl;
    for (auto &it: *hand.cards_) {
        os << "\t" << *it << std::endl;
    }
    return os;
}
