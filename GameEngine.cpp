#include "GameEngine.h"

GameEngine::GameEngine(std::string initState) : state(new std::string(initState)) {}
// Copy constructor
GameEngine::GameEngine(const GameEngine &rhs) : state(new std::string(*rhs.state)) {}
// Destructor
GameEngine::~GameEngine(){};

std::string GameEngine::getState()
{
    return *state;
}
