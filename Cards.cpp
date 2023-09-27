#include "Cards.h"

Card::Card(const std::string& cardType) {
    this->cardType = new std::string(cardType);
}

Card::Card(const Card& cardToCopy) {
    this->cardType = cardToCopy.cardType;
}

Card::~Card() {
    delete cardType;
}

std::string Card::getCardType() const {
    return *cardType;
}

Hand::Hand() {
    cardCollection = new std::vector<Card>();
}

Hand::Hand(const std::vector<Card>& cardCollection) {
    this->cardCollection = new std::vector<Card>(cardCollection);
}

Hand::Hand(const Hand& handToCopy) {
    this->cardCollection = handToCopy.cardCollection
}

Hand::~Hand() {
    delete cardCollection;
}

Deck::Deck() {
    this->cardDeck = new std::vector<Card>();
}

Deck::Deck(const std::vector<Card> &cardDeck) {
    this->cardDeck = new std::vector<Card>(cardDeck);
}


Deck::Deck(const Deck& deckToCopy) {
    this->cardDeck = deckToCopy.cardDeck;
}

Deck::~Deck() {
    delete cardDeck;
}

// TO DO
void Deck::draw() {}

// TO DO
void Card::play() {}