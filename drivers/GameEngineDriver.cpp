#include <iostream>
#include "../GameEngine.h"

// Daniel Soldera
// Carson Senthilkumar
// Joe El-Khoury
// Henri Stephane Carbon
// Haris Mahmood

/**
 * This is the method used to test the game engine state transitions, it takes a string
 * input from the user and passes it to the transition method which changes the state if
 * a valid command is given.
 */
void testGameStates() {
    std::cout << "Testing game state setup and transitions..." << std::endl;
    CommandProcessor *processor = new CommandProcessor();
    GameEngine engine = GameEngine(GameState::start, *processor);
    engine.gameLoop();
    delete processor;
}

void testGameStartup() {
//    std::cout << "Testing game startup: console mode" << std::endl;
    CommandProcessor *processor = new CommandProcessor();
    GameEngine engine = GameEngine(GameState::start, *processor);
//    engine.startup();
    delete processor;

    std::cout << "Testing game startup: file mode" << std::endl;
    processor = new FileCommandProcessorAdapter("res/gameStartup.txt");
    engine = GameEngine(GameState::start, *processor);
    engine.startup();
    // force win condition
    engine.transition(GameState::win);
    engine.gameOverPhase();

    // replay
    engine.startup();
    // force win condition
    engine.transition(GameState::win);
    engine.gameOverPhase();

    delete processor;
}