#include <iostream>
#include "../GameEngine.h"

// Daniel Soldera
// Carson Senthilkumar
// Joe El-Khoury
// Henri Stephane Carbon
// Haris Mahmood

// This is the method used to test the game engine state transitions, it takes a string input from the user and passes it to the transition method which changes the state if a valid command is given.
void testGameStates()
{
    std::cout << "Testing game state setup and transitions..." << std::endl;

    GameEngine engine = GameEngine(GameState::start);
    engine.gameLoop();
}