#include <iostream>
#include <random>
#include "../Cards.h"

// Daniel Soldera
// Carson Senthilkumar
// Joe El-Khoury
// Henri Stephane Carbon
// Haris Mahmood

void testCards() {
    std::random_device r;
    std::cout << "Testing the implementation of Cards" << std::endl;
    std::cout << "===================================" << std::endl;
    std::cout << "Creating New Cards" << std::endl;
    std::cout << "===================================" << std::endl;

    // Creating cards
    auto card1{std::make_shared<Card>(CardType::bomb)};
    auto card2{std::make_shared<Card>(CardType::reinforcement)};
    auto card3{std::make_shared<Card>(CardType::blockade)};
    auto card4{std::make_shared<Card>(CardType::airlift)};
    auto card5{std::make_shared<Card>(CardType::diplomacy)};

    auto card6{std::make_shared<Card>(CardType::bomb)};

    std::cout<<std::endl;
    std::cout << "===================================" << std::endl;
    std::cout << "Creating a Deck and adding Cards to it" << std::endl;
    std::cout << "===================================" << std::endl;

    // Creates an empty deck of cards
    // And adds the created cards to the deck
    Deck gameDeck{};
    gameDeck.add(card1);
    gameDeck.add(card2);
    gameDeck.add(card3);
    gameDeck.add(card4);
    gameDeck.add(card5);

    std::cout<<std::endl;
    std::cout << gameDeck << std::endl;

    std::cout << "===================================" << std::endl;
    std::cout << "Creating a Hand and drawing from the deck" << std::endl;
    std::cout << "===================================" << std::endl;

    // Creates a Hand by drawing cards from the deck
    Hand playerHand{};
    while(!gameDeck.empty()) {
        std::cout << playerHand << std::endl;
        std::cout << gameDeck << std::endl;
        gameDeck.draw(playerHand);
    }
    playerHand.add(card6);

    // Gets the number of cards from the hand
    std::cout << "Player has " << playerHand.size() << " cards." << std::endl;

    std::cout << std::endl;
    std::cout << "===================================" << std::endl;
    std::cout << "Playing the Cards from the hand" << std::endl;
    std::cout << "===================================" << std::endl;

    // Revised solution
    while (!playerHand.empty()) {
        std::cout << playerHand << std::flush;
        std::uniform_int_distribution<size_t> u(0, playerHand.size() - 1);
        std::default_random_engine e(r());
        auto random = u(e);

        //std::cout << "Index " << random << " is selected for playing card." << std::endl;
        auto cardSelected = playerHand.card(random);
        std::cout << "Playing " << *cardSelected << " at index " << random << std::endl;
        playerHand.remove(cardSelected);
        cardSelected->play();
        gameDeck.add(cardSelected);
        std::cout << std::endl;
    }

    std::cout << std::endl;
    std::cout << "===================================" << std::endl;
    std::cout << "Checking the cards from the Deck" << std::endl;
    std::cout << "===================================" << std::endl;

    auto cardAmountInDeck{gameDeck.size()};
    std::cout << gameDeck << std::endl;

}