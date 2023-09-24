#include "GameEngine.h"

GameEngine::GameEngine(const std::string &initState) : state(new std::string(initState)) {}
// Copy constructor
GameEngine::GameEngine(const GameEngine &rhs) : state(new std::string(*rhs.state)) {}
// Copying over state
GameEngine &GameEngine::operator=(const GameEngine &rhs)
{
    if (this != &rhs)
    { // in case we are re-assigning to ourselves i.e. f = f;
        // need to clean up current pointers because destructor won't be called.
        delete state;
        state = new std::string(*rhs.state);
    }

    return *this;
}
// Destructor
GameEngine::~GameEngine(){};

std::string GameEngine::getState()
{
    return *state;
}
