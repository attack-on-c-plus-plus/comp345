#include <iostream>
#include "../GameEngine.h"
using namespace std;

// This is the method used to test the game engine state transitions, it takes a string input from the user and passes it to the transition method which changes the state if a valid command is given.
void testGameStates()
{
    cout << "Testing game state setup and transitions..." << endl;

    const possibleStates init = start;
    GameEngine engine = GameEngine::GameEngine(init);
    string command;

    while (!engine.getGameOver())
    {
        cout << "Enter your command." << endl;
        cin >> command;
        engine.transaction(command);
    }
}