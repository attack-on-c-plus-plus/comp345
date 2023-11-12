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

#include "GameEngineDriver.h"

#include <iostream>

#include "../CommandProcessing.h"
#include "../GameEngine.h"
#include "../Map.h"

/**
 * This is the method used to test the game engine state transitions, it takes a string
 * input from the user and passes it to the transition method which changes the state if
 * a valid command is given.
 */
void testGameStates() {
    const std::string seperator(70, '=');
    std::cout << seperator << std::endl;
    std::cout << "Testing game state setup and transitions..." << std::endl;
    std::cout << seperator << std::endl;
    CommandProcessor processor;
    GameEngine engine = GameEngine(processor);
    engine.gameLoop();
}

void testGameStartup() {
    const std::string seperator(70, '=');
    std::cout << seperator << std::endl;
    std::cout << "Testing game startup: console mode" << std::endl;
    std::cout << seperator << std::endl;
    CommandProcessor commandProcessor;
    GameEngine engine = GameEngine(commandProcessor);
    engine.startup();

    std::cout << seperator << std::endl;
    std::cout << "Testing game startup: file mode" << std::endl;
    std::cout << seperator << std::endl;
    FileCommandProcessorAdapter fileCommandProcessorAdapter("res/gameStartup.txt");
    engine = GameEngine(fileCommandProcessorAdapter);
    engine.startup();
    // force win condition
    engine.transition(GameState::win);
    engine.gameOverPhase();

    // replay
    engine.startup();
    // force win condition
    engine.transition(GameState::win);
    engine.gameOverPhase();
}

void testMainGameLoop() {
    const std::string seperator(70, '=');
    std::cout << seperator << std::endl;
    std::cout << "Testing main game loop" << std::endl;
    std::cout << seperator << std::endl;
    FileCommandProcessorAdapter fileCommandProcessorAdapter("res/gameStartup.txt");
    GameEngine engine{fileCommandProcessorAdapter};
    engine.startup();

    for (const auto t : engine.map().territories()) {
        std::cout << *t << " owned by " << t->owner() << " has " << t->armyCount() << " armies" << std::endl;
    }

    engine.mainGameLoop();

    for (const auto t : engine.map().territories()) {
        std::cout << *t << " owned by " << t->owner() << " has " << t->armyCount() << " armies" << std::endl;
    }
}