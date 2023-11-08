#include <iostream>
#include <random>
#include "../Cards.h"

// Daniel Soldera
// Carson Senthilkumar
// Joe El-Khoury
// Henri Stephane Carbon
// Haris Mahmood

size_t randomInt(size_t min, size_t max);

void testCards() {
    CommandProcessor *processor = new CommandProcessor();
    GameEngine engine{GameState::start, *processor};
    Player player{};
    Territory territory{};
    Territory source{};
    Territory target{};
    unsigned int armies;
    std::cout << "Testing the implementation of Cards" << std::endl;
    std::cout << "===================================" << std::endl;
    std::cout << "Creating New Cards" << std::endl;
    std::cout << "===================================" << std::endl;

    // Creating cards
    
    Card card1{CardType::bomb};
    Card card2{CardType::reinforcement};
    Card card3{CardType::blockade};
    Card card4{CardType::airlift};
    Card card5{CardType::diplomacy};

    Card card6{CardType::bomb};

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
        auto hs = playerHand.size();
        auto random = randomInt(0, hs);

        auto cardSelected = playerHand.card(random);
        std::cout << "Playing " << cardSelected << " at index " << random << std::endl;
        gameDeck.discard(cardSelected.play(player, territory, source, target, armies, engine), playerHand);
        std::cout << std::endl;
    }

    std::cout << std::endl;
    std::cout << "===================================" << std::endl;
    std::cout << "Checking the cards from the Deck" << std::endl;
    std::cout << "===================================" << std::endl;

    auto cardAmountInDeck{gameDeck.size()};
    std::cout << gameDeck << std::endl;

    delete processor;
}

size_t randomInt(size_t min, size_t max) {
    std::random_device r;
    std::uniform_int_distribution<size_t> u(0, max - 1);
    std::default_random_engine e(r());
    return u(e);
}
