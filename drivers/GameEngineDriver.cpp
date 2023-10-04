#include <iostream>
#include "../GameEngine.h"

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