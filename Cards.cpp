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


#include "Cards.h"

#include <iostream>
#include <random>
#include <stdexcept>

#include "GameEngine.h"
#include "Map.h"
#include "Orders.h"
#include "Player.h"

/**
 * Creates a new card
 * @param type The card type
 */
Card::Card(const CardType type) : type_(new CardType(type)) {
    owner_ = nullptr;
}

/**
 * Creates a new Card by copying the card type of the card to be copied.
 * @param cardToCopy The card to be copied.
 */
Card::Card(const Card &cardToCopy) : type_{new CardType(*cardToCopy.type_)} {
    owner_ = cardToCopy.owner_;
}

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
CardType Card::type() const {
    return *type_;
}

/**
 * Play card
 */
Card& Card::play(std::istream&is) {
    // Check the card type. For each case, play an order
    unsigned targetId;

    // get a reference to owner before releasing the card.
    const auto owner = owner_;
    // Clear the cards owner
    owner_ = nullptr;
    std::cout << "Played card " << *this << std::endl;
    switch(*this->type_) {
        case CardType::bomb: {
            std::cout << "Available bomb territories: " << std::endl;
            for (const auto t: owner->toAttack()) {
                std::cout << "\t" << *t << " { owner: " << t->owner() << ", armies: " << t->armyCount() << " }" << std::endl;
            }
            is >> targetId;
            owner->orderList().addOrder(BombOrder{owner->gameEngine(), *owner, owner->gameEngine().map().territory(targetId)});
            break;
        }
        case CardType::blockade: {
            std::cout << "Available blockade territories: " << std::endl;
            for (const auto t: owner->territories()) {
                std::cout << "\t" << *t << " { armies: " << t->armyCount() << " }" << std::endl;
            }
            is >> targetId;
            owner->orderList().addOrder(BlockadeOrder{owner->gameEngine(), *owner, owner->gameEngine().map().territory(targetId)});
            break;
        }
        case CardType::airlift: {
            unsigned armies;
            unsigned sourceId;
            std::cout << "Available airlift source territories: " << std::endl;
            for (const auto t: owner->territories()) {
                std::cout << "\t" << *t << " { armies: " << t->armyCount() << " }" << std::endl;
            }
            is >> sourceId;
            Territory &source = owner->gameEngine().map().territory(sourceId);
            std::cout << "Available airlift target territories: " << std::endl;
            for (const auto t: owner->territories()) {
                std::cout << "\t" << *t << " { armies: " << t->armyCount() << " }" << std::endl;
            }
            is >> targetId;
            std::cout << "Armies: (available: " << source.armyCount() << ")" << std::endl;
            is >> armies;
            owner->orderList().addOrder(AirliftOrder{owner->gameEngine(), *owner, source, owner->gameEngine().map().territory(targetId), armies});
            break;
        }
        case CardType::diplomacy: {
            unsigned playerId;
            std::cout << "Available airlift source territories: " << std::endl;
            for (int i = 0; i < owner->gameEngine().players().size(); ++i) {
                std::cout << "\t" << i << ": " << owner->gameEngine().players().player(i) << std::endl;
            }
            is >> playerId;
            owner->orderList().addOrder(NegotiateOrder{owner->gameEngine(), *owner, owner->gameEngine().players().player(playerId)});
            break;
        }
        default: {
            throw std::out_of_range("invalid card type");
        }
    }
    return *this;
}

void Card::owner(const Player& player) {
    owner_ = &player;
}

/**
 * An equals operator that compares the cardType
 * Required to find the card from a vector of cards
 * @param card The card to compare
 * @return True if the card type is the same, and false otherwise.
 */
bool Card::operator==(const Card &card) const {
    return *this->type_ == *card.type_;
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
 * @param random
 * @param size The deck size. By default, the size is set to 5.
 */
Deck::Deck(const IRandom &random, const unsigned int size) {
    random_ = &random;
    cards_ = new std::vector<Card *>();
    cards_->reserve(size);
    for (int i = 0; i < size; ++i)
    {
        switch(static_cast<CardType>(i % 5)) {
            case CardType::bomb:
                this->add(CardType::bomb);
                break;
            case CardType::reinforcement:
                this->add(CardType::reinforcement);
                break;
            case CardType::blockade:
                this->add(CardType::blockade);
                break;
            case CardType::airlift:
                this->add(CardType::airlift);
                break;
            case CardType::diplomacy:
                this->add(CardType::diplomacy);
                break;
        }
    }
}

/**
 * A constructor for the Deck class
 * @param random
 * @param cardDeck an existing vector of Cards
 */
Deck::Deck(const IRandom &random, const std::vector<Card> &cardDeck) {
    random_ = &random;
    cards_ = new std::vector<Card *>();
    for (const auto &c: cardDeck) {
        cards_->push_back(new Card(c.type()));
    }
}

/**
 * Copy constructor for Deck
 * @param deckToCopy The deck to be copied.
 */
Deck::Deck(const Deck &deckToCopy) {
    random_ = deckToCopy.random_;
    cards_ = new std::vector<Card *>();
    for (const auto c: *deckToCopy.cards_) {
        cards_->push_back(new Card(c->type()));
    }
}

/**
 * Destructor for the Deck class. Deletes the cardDeck pointer.
 */
Deck::~Deck() {
    for (const auto c: *cards_) delete c;
    cards_->clear();
    delete cards_;
}

/**
 * Adds a card to the Deck.
 * @param cardType The card type to be added.
 * @return The Deck with the new card.
 */
Deck &Deck::add(const CardType cardType) {
    cards_->push_back(new Card(cardType));
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
Deck &Deck::draw(const Hand &hand) {
    if (cards_->empty()) {
        throw std::out_of_range("No more cards in the deck");
    }
    const auto index = random_->generate(0, cards_->size() - 1);
    auto *card = cards_->at(index);
    card->owner(hand.player());
    hand.cards_->push_back(card);
    cards_->erase(cards_->begin() + index);

    return *this;
}

/**
 * Discards a card back from the supplied hand back to this deck
 * @param card
 * @param fromHand
 * @return
 */
Deck &Deck::discard(const Card &card, const Hand &fromHand) {
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
const Card &Deck::card(const size_t index) const {
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
const std::vector<Card *> &Deck::cards() const {
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
        for (const auto c: *cards_) delete c;
        cards_->clear();
        delete cards_;
        cards_ = new std::vector<Card *>();
        for (const auto &c: *deck.cards_) {
            add(c->type());
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
    for (const auto &card: *deck.cards_) {
        os << "\t" << *card << std::endl;
    }
    return os;
}

/**
 * Creates a hand object of a specific size
 * @param player
 * @param size: The number of cards a hand can hold initially.
 */
Hand::Hand(const Player &player, const unsigned int size) :
        cards_{new std::vector<Card *>()} {
    player_ = &player;
    cards_->reserve(size);
}

/**
 * Creates a hand object by providing an existing card collection
 * @param player
 * @param cardCollection The card collection.
 */
Hand::Hand(const Player &player, const std::vector<Card> &cardCollection) {
    player_ = &player;
    cards_ = new std::vector<Card *>();
    for (const auto &c: cardCollection) {
        cards_->push_back(new Card(c));
    }
}

/**
 * Creates a hand by copying another hand.
 * @param handToCopy The hand to be copied.
 */
Hand::Hand(const Hand &handToCopy) {
    player_ = handToCopy.player_;
    cards_ = new std::vector<Card *>();
    for (const auto &c: *handToCopy.cards_) {
        cards_->push_back(new Card(*c));
    }
}

/**
 * Default Card destructor. Deletes the cardType pointer.
 */
Hand::~Hand() {
    for (const auto c: *cards_) delete c;
    delete cards_;
}

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
Card &Hand::card(const size_t index) const {
    return *cards_->at(index);
}

/**
 * Gets the number of cards in the player's hand
 * @return an int indicating the size of the player's hand
 */
size_t Hand::size() const {
    return cards_->size();
}

bool Hand::has(CardType type) const {
    auto has_type = [type](const Card* c) { return c->type() == type; };

    if (const auto it = std::ranges::find_if(*cards_, has_type); it != cards_->end())
        return true;
    return false;
}

/**
 * Gets if a deck is empty
 * @return true if empty; false otherwise
 */
bool Hand::empty() const {
    return cards_->empty();
}

const Player& Hand::player() const {
    return *player_;
}

/**
 * Gets the underlying cards in the deck
 * @return
 */
const std::vector<Card *> &Hand::cards() const {
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
        for (const auto c: *cards_) delete c;
        cards_->clear();
        delete cards_;
        cards_ = new std::vector<Card *>(hand.cards_->size());
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
    for (const auto &it: *hand.cards_) {
        os << *it << " ";
    }
    return os;
}