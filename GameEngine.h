#ifndef COMP345_GAMEENGINE_H
#define COMP345_GAMEENGINE_H

#include <string>

/*
GameEngine.cpp/GameEngine.h implement the system that tracks the current state of the game and which methods are supposed to be run in each state. The states form the structure of the setup and the game itself.
*/

// Basic definition to avoid definition order within file.
class GameEngine;

/*
GameEngine is implemented as a container for two strings, one representing the state and the other a command given by the user to be used as an argument for a method that is also stored in the class.
*/
class GameEngine
{
public:
    explicit GameEngine(const std::string &initState);
    // Copy constructor
    GameEngine(const GameEngine &rhs);

    GameEngine &operator=(const GameEngine &rhs);
    // Destructor
    ~GameEngine();
    void transaction();
    [[nodiscard]] std::string getState();
    [[nodiscard]] std::string getCommand() const;

private:
    std::string *state;
};

#endif // COMP345_GAMEENGINE_H