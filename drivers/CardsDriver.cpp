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
    auto *processor = new CommandProcessor();
    GameEngine engine{GameState::start, *processor};
    Player player{"Bob"};
    Territory territory{};
    std::cout << "Testing the implementation of Cards" << std::endl;
    std::cout << "===================================" << std::endl;
    std::cout << "Creating New Cards" << std::endl;
    std::cout << "===================================" << std::endl;

<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
    Card card6{CardType::bomb};
=======
    // Creating cards
    
    Card card1{CardType::bomb};
<<<<<<< HEAD
<<<<<<< HEAD
    Card card2{CardType::advance};
    Card card3{CardType::deploy};
    Card card4{CardType::blockade};
    Card card5{CardType::airlift};
    Card card6{CardType::diplomacy};
>>>>>>> 3feb183 (updated play function in Card's driver class to satisfy all card types)
=======
=======
>>>>>>> ebbb955 (Update CardsDriver.cpp)
    Card card2{CardType::reinforcement};
    Card card3{CardType::blockade};
    Card card4{CardType::airlift};
    Card card5{CardType::diplomacy};

=======
>>>>>>> 50240d0 (updaing remote branch)
=======
>>>>>>> 50240d095beac2bba48406e50f84aa1489e5c2bd
    Card card6{CardType::bomb};
<<<<<<< HEAD

>>>>>>> c4649cd (fixed advanced order to properly give attacker a card if he wins battle)
=======
>>>>>>> ebbb955 (Update CardsDriver.cpp)

    std::cout<<std::endl;
    std::cout << "===================================" << std::endl;
    std::cout << "Creating a Deck and adding Cards to it" << std::endl;
    std::cout << "===================================" << std::endl;

    // Creates an empty deck of cards
    // And adds the created cards to the deck
    Deck gameDeck{};
    gameDeck.add(CardType::bomb);
    gameDeck.add(CardType::reinforcement);
    gameDeck.add(CardType::blockade);
    gameDeck.add(CardType::airlift);
    gameDeck.add(CardType::diplomacy);
<<<<<<< HEAD
<<<<<<< HEAD

=======
>>>>>>> 50240d0 (updaing remote branch)
=======
>>>>>>> 50240d095beac2bba48406e50f84aa1489e5c2bd

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
        gameDeck.discard(cardSelected.play(player, territory,engine), playerHand);
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