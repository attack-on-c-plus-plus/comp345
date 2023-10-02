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

Hand &Hand::addCard(const Card &card) {
    cardCollection->emplace_back(card);
    return *this;
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

void Deck::draw(Hand& hand) {
    if (this->cardDeck->empty()) {
        std::cout<<"No more cards in the deck"<<std::endl;
        return;
    }

    /*
    auto: C++ feature that automatically detects and assigns
    a data type to the variable according to the initialization
    expression.
    */
    auto random = rand() % cardDeck->size();
    hand.addCard(cardDeck->at(random));
    cardDeck->erase(cardDeck->begin() + random);

}

// TO DO
void Card::play() {
    
}