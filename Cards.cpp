#include "Cards.h"
#include <iostream>
#include <algorithm>

// Daniel Soldera
// Carson Senthilkumar
// Joe El-Khoury
// Henri Stephane Carbon
// Haris Mahmood

/**
 * Creates a new card
 * @param cardType The card type
 */
Card::Card(const std::string& cardType) : cardType(new std::string(cardType)) {
    std::cout << "Creating a new card " << this << " of type: " + cardType << std::endl;
}

/**
 * Creates a new Card by copying the card type of the card to be copied.
 * @param cardToCopy The card to be copied.
 */
Card::Card(const Card& cardToCopy) : cardType(new std::string(*cardToCopy.cardType)) {
    std::cout << "Creating a copy of the card " << &cardToCopy
    << " that has the type: " + cardToCopy.getCardType() <<std::endl;
}

/**
 * Default Card destructor. Deletes the cardType pointer.
 */
Card::~Card() {
    std::cout << "Destroying card " << this << std::endl;
    delete cardType;
}

/**
 * Gets a card from the player's hand
 * @param index The index of the selected card from the player's card collection.
 * @return The selected card
 */
Card &Hand::getCard(unsigned int index) {
    return cardCollection->at(index);
}

/**
 * Gets the number of cards in the player's hand
 * @return an int indicating the size of the player's hand
 */
unsigned int Hand::getCardCount() const {
    return cardCollection->size();
}

/**
 * Gets the card type
 * @return A string that represents the card type.
 */
std::string Card::getCardType() const {
    return *cardType;
}

/**
 * Creates a hand object of a specific size
 * @param collectionSize: The number of cards a hand can hold initially.
 */
Hand::Hand(const int &collectionSize) : cardCollection(new std::vector<Card>()) {
    cardCollection->reserve(collectionSize);
}

/**
 * Creates a hand object by providing an existing card collection
 * @param cardCollection The card collection.
 */
Hand::Hand(const std::vector<Card>& cardCollection) {
    this->cardCollection = new std::vector<Card>(cardCollection);
}

/**
 * Creates a hand by copying another hand.
 * @param handToCopy The hand to be copied.
 */
Hand::Hand(const Hand& handToCopy) {
    this->cardCollection = new std::vector<Card>(*handToCopy.cardCollection);
}

/**
 * Default Card destructor. Deletes the cardType pointer.
 */
Hand::~Hand() {
    delete cardCollection;
}

/**
 * Adds a card to the hand.
 * @param card The card to be added
 * @return The hand with the new card.
 */
Hand &Hand::addCard(const Card &card) {
    std::cout << "Pushing back" << std::endl;
    cardCollection->push_back(card);
    return *this;
}

/**
 * Removes the card from the hand.
 * @param card
 * @return The hand with the removed card.
 */
Hand &Hand::removeCard(const unsigned &index) {
    cardCollection->erase(cardCollection->begin() + index);
    return *this;
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
        delete cardCollection;
        cardCollection = new std::vector<Card>(*hand.cardCollection);
    }
    return *this;
}

// TO DO
//std::istream &operator>>(std::istream &is, Hand &hand) {}

/**
 * Output stream when printing hand to console
 * @param os The output stream
 * @param hand The hand object
 * @return the output stream
 */
std::ostream &operator<<(std::ostream &os, Hand &hand) {
    for (int i = 0; i < hand.getCardCount(); i++) {
        os << "Hand has a card of type: " << hand.getCard(i).getCardType();
    }
    return os;
}

/**
 * A constructor for the Deck class
 * @param cardDeck an existing vector of Cards
 */
Deck::Deck(const std::vector<Card> &cardDeck) {
    this->cardDeck = new std::vector<Card>(cardDeck);
}

/**
 * Adds a card to the Deck.
 * @param card The card to be added
 * @return The Deck with the new card.
 */
Deck &Deck::addCard(const Card &card){
    std::cout << "Adding card " + card.getCardType() << " to deck." << std::endl;
    cardDeck->push_back(card);
    return *this;
}

/**
 * Removes a card from the Deck
 * @param index The index where the card is located
 * @return The deck with the removed card.
 */
Deck &Deck::removeCard(const unsigned &index){
    cardDeck->erase(cardDeck->begin() + index);
    return *this;
}

/**
 * Copy constructor for Deck
 * @param deckToCopy The deck to be copied.
 */
Deck::Deck(const Deck& deckToCopy) {
    this->cardDeck = new std::vector<Card>(*deckToCopy.cardDeck);
}

/**
 * Destructor for the Deck class. Deletes the cardDeck pointer.
 */
Deck::~Deck() {
    delete cardDeck;
}

/**
 * Draw cards from the deck and place it directly into a hand
 * @param hand The hand where the randomly chosen card will be placed.
 */
void Deck::draw(Hand& hand) {
    if (this->cardDeck->empty()) {
        std::cout << "No more cards in the deck" << std::endl;
        return;
    }

    auto random = rand() % cardDeck->size();

    std::cout << "Card from deck is " << &(cardDeck->at(random)) << std::endl;
    hand.addCard(cardDeck->at(random));
    this->removeCard(random);
}

/**
 * Deck constructor that initializes a deck of a specified size.
 * @param deckSize The deck size. By default, the size is set to 5.
 */
Deck::Deck(const int &deckSize) {
    this->cardDeck = new std::vector<Card>();
    cardDeck->reserve(deckSize);
}

/**
 * Gets the number of cards in the Deck
 * @return an int indicating the number of cards
 */
unsigned int Deck::getCardCount() const {
    return cardDeck->size();
}

/**
 * Gets a card from a deck
 * @param index The index of the selected card from the Deck of cards.
 * @return The selected card
 */
Card &Deck::getCard(unsigned int index) {
    return cardDeck->at(index);
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
        delete cardDeck;
        cardDeck = new std::vector<Card>(*deck.cardDeck);
    }
    return *this;
}

// TO DO
//std::istream &operator>>(std::istream &is, Deck &deck) {}

std::ostream &operator<<(std::ostream &os, Deck &deck) {
    for (int i = 0; i < deck.getCardCount(); i++) {
        os << "Deck has a card of type: " << deck.getCard(i).getCardType();
    }
    return os;
}

/**
 * Creates a special order
 */
void Card::play() {
    // Special orders logic here
    std::cout << "Played card " << *this->cardType;
}

/**
 * An equals operator that compares the cardType
 * Required to find the card from a vector of cards
 * @param card The card to compare
 * @return True if the card type is the same, and false otherwise.
 */
bool Card::operator==(const Card &card) const {
    return (*this->cardType == *card.cardType);
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
        delete cardType;
        cardType = new std::string(*card.cardType);
    }
    return *this;
}

// TO DO
//std::istream &operator>>(std::istream &is, Card &card) {}

/**
 * Output stream operator of Card
 * @param os The output stream
 * @param card A card
 * @return the output stream itself
 */
std::ostream &operator<<(std::ostream &os, Card &card) {
    os << "Card is of type: " << card.cardType << std::endl;
    return os;
}
