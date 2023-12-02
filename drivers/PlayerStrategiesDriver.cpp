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

#include "PlayerStrategiesDriver.h"
#include "../PlayerStrategies.h"
#include "../GameEngine.h"
#include "../Player.h"
#include "../Map.h"
#include "../CommandProcessing.h"
#include <iostream>

void testPlayerStrategies() {
    const FakeRandom random;

    FileCommandProcessorAdapter fileCommandProcessorAdapter("res/playerStrategiesStartUp.txt");
    GameEngine gameEngine{fileCommandProcessorAdapter, random};
    gameEngine.startup();

    const std::string separator(70, '=');

//------------------------------------------------------------------------------------------
    // Create a Human player and demonstrate its behavior
    std::cout << separator << std::endl;
    std::cout << "Testing Human Player Strategy" << std::endl;
    std::cout << separator << std::endl;

    std::cout << gameEngine.players() << std::endl;
//    std::cout << toString(gameEngine.players().player(0).strategy()) << std::endl;
    gameEngine.players().issueOrders();
    gameEngine.map().print();
    // Demonstrate Human player behavior
    std::cout << "Initial State:" << std::endl;

//    humanPlayer.issueOrders();
//    std::cout << "After Issuing Orders:" << std::endl;
//    std::cout << humanPlayer << std::endl;
////------------------------------------------------------------------------------------------
//    // Create a Benevolent player and demonstrate its behavior
//    std::cout << separator << std::endl;
//    std::cout << "Testing Benevolent Player Strategy" << std::endl;
//    std::cout << separator << std::endl;
//
//    Player benevolentPlayer{gameEngine, "Benevolent", Strategy::Benevolent};
//    BenevolentPlayerStrategy benevolentStrategy{benevolentPlayer, gameEngine};
//
//    // Demonstrate Benevolent player behavior
//    std::cout << "Initial State:" << std::endl;
//    std::cout << benevolentPlayer << std::endl;
//    benevolentPlayer.issueOrders();
//    std::cout << "After Issuing Orders:" << std::endl;
//    std::cout << benevolentPlayer << std::endl;
////------------------------------------------------------------------------------------------
//    // Create a Neutral player and demonstrate its behavior
//    std::cout << separator << std::endl;
//    std::cout << "Testing Neutral Player Strategy" << std::endl;
//    std::cout << separator << std::endl;
//
//    Player neutralPlayer{gameEngine, "Neutral", Strategy::Neutral};
//    NeutralPlayerStrategy neutralStrategy{neutralPlayer, gameEngine};
//
//    // Demonstrate Neutral player behavior
//    std::cout << "Initial State:" << std::endl;
//    std::cout << neutralPlayer << std::endl;
//    neutralPlayer.issueOrders();  // Neutral player takes a balanced approach
//    std::cout << "After Issuing Orders:" << std::endl;
//    std::cout << neutralPlayer << std::endl;
////------------------------------------------------------------------------------------------
//    // Create a default Player and demonstrate its behavior
//    std::cout << separator << std::endl;
//    std::cout << "Testing Default Player Strategy" << std::endl;
//    std::cout << separator << std::endl;

//    //Issue: Theres no way of creating a default player????
//    Player defaultPlayer{gameEngine, "Default"};

//    // Demonstrate Default player behavior
//    std::cout << "Initial State:" << std::endl;
//    std::cout << defaultPlayer << std::endl;
//    defaultPlayer.issueOrders();  // Default player has a basic strategy
//    std::cout << "After Issuing Orders:" << std::endl;
//    std::cout << defaultPlayer << std::endl;
//------------------------------------------------------------------------------------------

}
