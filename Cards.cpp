#include "Cards.h"
#include <iostream>
#include <algorithm>

Card::Card(const std::string& cardType) : cardType(new std::string(cardType)) {
    std::cout << "Creating a new card " << this << " of type: " + cardType << std::endl;
}

Card::Card(const Card& cardToCopy) : cardType(new std::string(*cardToCopy.cardType)) {
    std::cout << "Creating a copy of the card " << &cardToCopy
    << " that has the type: " + cardToCopy.getCardType() <<std::endl;
}

Card::~Card() {
    std::cout << "Destroying card " << this << std::endl;
    delete cardType;
}

Card &Hand::getCard(unsigned int index) {
    return cardCollection->at(index);
}

unsigned int Hand::getCardCount() const {
    return cardCollection->size();
}

std::string Card::getCardType() const {
    return *cardType;
}

Hand::Hand(const int &collectionSize) : cardCollection(new std::vector<Card>()) {
    cardCollection->reserve(collectionSize);
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
    std::cout << "Pushing back" << std::endl;
    try {
        cardCollection->push_back(card);
    } catch (std::exception& e) {
        std::cout<<e.what()<<std::endl;
        exit(3);
    }
    return *this;
}

Hand &Hand::removeCard(const Card &card) {
    // Find the index of the referred card object
    auto it = std::find(cardCollection->begin(), cardCollection->end(), card);
    if (it == cardCollection->end()) {
        std::cout << "Card not found from hand." << std::endl;
        return *this;
    }
    // Also causes issues here.
    cardCollection->erase(it);
    return *this;
}

Hand &Hand::operator=(const Hand &hand) {
    if (this != &hand) {
        delete cardCollection;
        cardCollection = new std::vector<Card>(*hand.cardCollection);
    }
    return *this;
}

Deck::Deck(const std::vector<Card> &cardDeck) {
    this->cardDeck = new std::vector<Card>(cardDeck);
}

Deck &Deck::addCard(const Card &card){
    std::cout << "Adding card " + card.getCardType() << " to deck." << std::endl;
    cardDeck->push_back(card);
    return *this;
}

Deck &Deck::removeCard(const unsigned &index){
    cardDeck->erase(cardDeck->begin() + index);
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
        std::cout << "No more cards in the deck" << std::endl;
        return;
    }

    /*
    auto: C++ feature that automatically detects and assigns
    a data type to the variable according to the initialization
    expression.
    */
    std::cout << "Deck size is " << cardDeck->size() << std::endl;
    std::cout << "Drawing from deck" << std::endl;
    auto random = rand() % cardDeck->size();
    std::cout << "Random calculated to be " << random << std::endl;

    // Create a pointer that points to the memory address of the card
    // to be erased from deck and placed to player hand.
    std::cout << "Card from deck is " << &(cardDeck->at(random)) << std::endl;
    hand.addCard(cardDeck->at(random));
    this->removeCard(random);
}

Deck::Deck(const int &deckSize) {
    this->cardDeck = new std::vector<Card>();
    cardDeck->reserve(deckSize);
}

unsigned int Deck::getCardCount() const {
    return cardDeck->size();
}

Card &Deck::getCard(unsigned int index) {
    return cardDeck->at(index);
}

Deck &Deck::operator=(const Deck &deck) {
    if (this != &deck) {
        delete cardDeck;
        cardDeck = new std::vector<Card>(*deck.cardDeck);
    }
    return *this;
}

/**
 * Plays the card from the player's hand and returns it
 * to the deck.
 * @param hand: the player's hand where the card is.
 * @param deck: the deck where the card will be returned to
 */
void Card::play() {
    // Special orders logic
    std::cout << "Played card " << *this->cardType;
}

bool Card::operator==(const Card &card) const {
    return (*this->cardType == *card.cardType);
}

Card &Card::operator=(const Card &card) {
    if (this != &card) {
        delete cardType;
        cardType = new std::string(*card.cardType);
    }
    return *this;
}
