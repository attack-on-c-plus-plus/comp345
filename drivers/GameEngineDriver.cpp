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
    const FakeRandom random;
    CommandProcessor processor;
    GameEngine engine{processor, random};
    engine.gameLoop();
}

void testGameStartup() {
    const std::string seperator(70, '=');
    std::cout << seperator << std::endl;
    std::cout << "Testing game startup: console mode" << std::endl;
    std::cout << seperator << std::endl;
    const FakeRandom random;
    CommandProcessor commandProcessor;
    GameEngine engine{commandProcessor, random};
    engine.startup();

    std::cout << seperator << std::endl;
    std::cout << "Testing game startup: file mode" << std::endl;
    std::cout << seperator << std::endl;
    FileCommandProcessorAdapter fileCommandProcessorAdapter("res/gameStartup.txt");
    engine = GameEngine(fileCommandProcessorAdapter, random);
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
    const FakeRandom random;
    FileCommandProcessorAdapter fileCommandProcessorAdapter("res/gameStartup.txt");
    GameEngine engine{fileCommandProcessorAdapter, random};
    engine.startup();

    engine.mainGameLoop();

    engine.map().print();
}