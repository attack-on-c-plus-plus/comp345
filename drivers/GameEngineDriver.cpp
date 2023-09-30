#include <iostream>
#include "../GameEngine.h"
using namespace std;

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