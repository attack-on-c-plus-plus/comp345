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

#include <iostream>

#include "../CommandProcessing.h"
#include "../GameEngine.h"

/**
 * This is the method used to test the game engine state transitions, it takes a string
 * input from the user and passes it to the transition method which changes the state if
 * a valid command is given.
 */
void testGameStates() {
    std::cout << "Testing game state setup and transitions..." << std::endl;
    CommandProcessor processor;
    GameEngine engine = GameEngine(processor);
    engine.gameLoop();
}

void testGameStartup() {
    std::cout << "Testing game startup: console mode" << std::endl;
    CommandProcessor commandProcessor;
    GameEngine engine = GameEngine(commandProcessor);
    engine.startup();

    std::cout << "Testing game startup: file mode" << std::endl;
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