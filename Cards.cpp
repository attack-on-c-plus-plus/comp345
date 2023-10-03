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

Hand &Hand::removeCard(const Card& card){
    // Find the index of the referred card object 
    auto it = std::find(cardCollection->begin(), cardCollection->end(), card);
    if (it != cardCollection->end()) {
        std::cout<<"Card not found from hand"<<std::endl;
        return *this;
    }
    cardCollection->erase(it);
    return *this;
}

Deck::Deck() {
    this->cardDeck = new std::vector<Card>();
}

Deck::Deck(const std::vector<Card> &cardDeck) {
    this->cardDeck = new std::vector<Card>(cardDeck);
}

Deck &Deck::addCard(const Card &card){
    cardDeck->emplace_back(card);
    return *this;
}

Deck &Deck::removeCard(const Card &card){
    auto it = std::find(cardDeck->begin(), cardDeck->end(), card);
    // Check if card exists
    if (it != cardDeck->end()) {
        std::cout<<"Card not found."<<std::endl;
        return *this;
    }
    cardDeck->erase(it);
    return *this;
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
void Card::play(Hand& hand, Deck& deck) {
    // Special orders logic
    
    // Then, remove card from hand
    hand.removeCard(*this);

    // And put it back in the deck
    deck.addCard(*this);

}