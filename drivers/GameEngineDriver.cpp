#include <iostream>
#include "../GameEngine.h"

// Daniel Soldera (40168674)

// This is the method used to test the game engine state transitions, it takes a string input from the user and passes it to the transition method which changes the state if a valid command is given.
void testGameStates()
{
    std::cout << "Testing game state setup and transitions..." << std::endl;

    const possibleStates init = start;
    GameEngine engine = GameEngine(init);
    std::string command;

    while (!engine.getGameOver())
    {
        std::cout << "Enter your command." << std::endl;
        std::cin >> command;
        engine.transaction(command);
    }
}