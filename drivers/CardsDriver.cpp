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

#include "CardsDriver.h"

#include <iostream>

#include "../Cards.h"
#include "../CommandProcessing.h"
#include "../GameEngine.h"
#include "../Map.h"
#include "../Player.h"

void testCards() {
    const std::string seperator(70, '=');
    auto *processor = new CommandProcessor();
    GameEngine engine{*processor};
    Player player{engine, "Bob"};
    Territory territory{};
    std::cout << seperator << std::endl;
    std::cout << "Testing the implementation of Cards" << std::endl;
    std::cout << seperator << std::endl;
    std::cout << "Creating New Cards" << std::endl;
    std::cout << seperator << std::endl;

    const Card card6{CardType::bomb};

    std::cout<<std::endl;
    std::cout << seperator << std::endl;
    std::cout << "Creating a Deck and adding Cards to it" << std::endl;
    std::cout << seperator << std::endl;

    // Creates an empty deck of cards
    // And adds the created cards to the deck
    Deck gameDeck{};
    gameDeck.add(CardType::bomb);
    gameDeck.add(CardType::reinforcement);
    gameDeck.add(CardType::blockade);
    gameDeck.add(CardType::airlift);
    gameDeck.add(CardType::diplomacy);

    std::cout<<std::endl;
    std::cout << gameDeck << std::endl;

    std::cout << seperator << std::endl;
    std::cout << "Creating a Hand and drawing from the deck" << std::endl;
    std::cout << seperator << std::endl;

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
    std::cout << seperator << std::endl;
    std::cout << "Playing the Cards from the hand" << std::endl;
    std::cout << seperator << std::endl;

    // Revised solution
    while (!playerHand.empty()) {
        std::cout << playerHand << std::flush;
        Random rnd;
        const auto random = rnd.generate(0, playerHand.size() - 1);

        const auto& cardSelected = playerHand.card(random);
        std::cout << "Playing " << cardSelected << " at index " << random << std::endl;
        gameDeck.discard(cardSelected.play(player, territory,engine), playerHand);
        std::cout << std::endl;
    }

    std::cout << std::endl;
    std::cout << seperator << std::endl;
    std::cout << "Checking the cards from the Deck" << std::endl;
    std::cout << seperator << std::endl;

    std::cout << gameDeck << std::endl;

    delete processor;
}