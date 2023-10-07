#include <iostream>
#include "../Cards.h"

// Daniel Soldera
// Carson Senthilkumar
// Joe El-Khoury
// Henri Stephane Carbon
// Haris Mahmood

void testCards() {
    std::cout << "Testing the implementation of Cards" << std::endl;
    std::cout << "===================================" << std::endl;
    std::cout << "Creating New Cards" << std::endl;
    std::cout << "===================================" << std::endl;

    // Creating cards
    int nbCards = 5;
    Card card1{"bomb"};
    Card card2{"reinforcement"};
    Card card3{"blockade"};
    Card card4{"airlift"};
    Card card5{"diplomacy"};

    std::cout<<std::endl;
    std::cout << "===================================" << std::endl;
    std::cout << "Creating a Deck and adding Cards to it" << std::endl;
    std::cout << "===================================" << std::endl;

    // Creates an empty deck of cards
    // And adds the created cards to the deck
    Deck gameDeck{};
    gameDeck.addCard(card1);
    gameDeck.addCard(card2);
    gameDeck.addCard(card3);
    gameDeck.addCard(card4);
    gameDeck.addCard(card5);

    std::cout<<std::endl;

    auto currentAmountInDeck{gameDeck.getCardCount()};
    for (int i = 0; i < currentAmountInDeck; i++) {
        std::cout << "Deck has a card " << &(gameDeck.getCard(i)) << " of type: "
        << (gameDeck.getCard(i)).getCardType() << std::endl;
    }

    std::cout << std::endl;
    std::cout << "===================================" << std::endl;
    std::cout << "Creating a Hand and drawing from the deck" << std::endl;
    std::cout << "===================================" << std::endl;

    // Creates a Hand by drawing cards from the deck
    Hand playerHand{};
    for (int i = 0; i < nbCards; i++) {
        gameDeck.draw(playerHand);
    }

    // Gets the number of cards from the hand
    std::cout << "Player has " << playerHand.getCardCount() << " cards." << std::endl;

    std::cout << std::endl;
    std::cout << "===================================" << std::endl;
    std::cout << "Playing the Cards from the hand" << std::endl;
    std::cout << "===================================" << std::endl;

    // Revised solution
    while (playerHand.getCardCount() > 0) {
        auto random {rand() % playerHand.getCardCount()};
        std::cout << "Index " << random << " is selected for playing card." << std::endl;
        Card cardSelected = playerHand.getCard(random);
        playerHand.removeCard(random);
        cardSelected.play();
        gameDeck.addCard(cardSelected);
    }

    std::cout << std::endl;
    std::cout << "===================================" << std::endl;
    std::cout << "Checking the cards from the Deck" << std::endl;
    std::cout << "===================================" << std::endl;

    auto cardAmountInDeck{gameDeck.getCardCount()};
    for (int i = 0; i < cardAmountInDeck; i++) {
        std::cout << "Deck has a card " << &(gameDeck.getCard(i)) << " of type: "
        << (gameDeck.getCard(i)).getCardType() << std::endl;
    }

}