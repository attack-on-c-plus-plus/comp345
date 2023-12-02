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

    std::cout << "Players: "<<gameEngine.players() <<std::endl;
    std::cout << "Strategies Employed: "<<std::endl;
    std::cout << gameEngine.players().player(0)<<": "<<toString(gameEngine.players().player(0).strategy()) << std::endl;
    std::cout << gameEngine.players().player(1)<<": "<<toString(gameEngine.players().player(1).strategy()) << std::endl;
    std::cout << gameEngine.players().player(2)<<": "<<toString(gameEngine.players().player(2).strategy()) << std::endl;
    std::cout << gameEngine.players().player(3)<<": "<<toString(gameEngine.players().player(3).strategy()) << std::endl;
    std::cout << separator << std::endl;

    gameEngine.players().issueOrders();

    std::cout << separator << std::endl;
    std::cout << "Execute Order:" << std::endl;
    gameEngine.players().executeOrders();
    std::cout << separator << std::endl;

    std::cout << separator << std::endl;
    std::cout << "Map Info: Before Reinforcement" << std::endl;
    gameEngine.map().print();

    std::cout << "\nAfter Reinforcement" << std::endl;
    gameEngine.transition(GameState::assignReinforcements);
    gameEngine.players().reinforcement();
    gameEngine.map().print();


    std::cout << separator << std::endl;

    std::cout << "\n"<< separator << std::endl;
    std::cout << "Neutral Player Testing "<<std::endl;
    std::cout << separator << std::endl;

    std::cout <<"\n"<<  separator << std::endl;
    std::cout << "Benevolent Player Testing "<<std::endl;
    std::cout << separator << std::endl;


    std::cout <<"\n"<<  separator << std::endl;
    std::cout << "Aggressive Player Testing "<<std::endl;
    std::cout << separator << std::endl;

    std::cout <<"\n"<< separator << std::endl;
    std::cout << "Cheater Player Testing "<<std::endl;
    std::cout << separator << std::endl;

//------------------------------------------------------------------------------------------

}
