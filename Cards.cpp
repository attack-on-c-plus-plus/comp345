#include "Cards.h"
#include <iostream>

Card::Card(const std::string& cardType) {
    this->cardType = new std::string(cardType);
}

Card::Card(const Card& cardToCopy) {
    this->cardType = new std::string(*cardToCopy.cardType);
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
    this->cardCollection = new std::vector<Card>(*handToCopy.cardCollection);
}

Hand::~Hand() {
    delete cardCollection;
}

std::vector<Card> * Hand::getCardCollection() const {
    return cardCollection;
}

Deck::Deck() {
    this->cardDeck = new std::vector<Card>();
}

Deck::Deck(const std::vector<Card> &cardDeck) {
    this->cardDeck = new std::vector<Card>(cardDeck);
}


Deck::Deck(const Deck& deckToCopy) {
    this->cardDeck = new std::vector<Card>(*deckToCopy.cardDeck);
}

Deck::~Deck() {
    delete cardDeck;
}

// TO DO
void Deck::draw(const Hand& hand) {
    if (this->cardDeck->empty()) {
        std::cout<<"No more cards in the deck"<<std::endl;
        return;
    }

    long *random = reinterpret_cast<long *>(rand() % cardDeck->size());
    Card* chosenCard = new Card((*cardDeck)[reinterpret_cast<unsigned long long int>(random)]);

    hand.getCardCollection()->push_back(*chosenCard);

    cardDeck->erase(cardDeck->begin() + *random);

    delete random;
    delete chosenCard;
}

// TO DO
void Card::play() {
    
}