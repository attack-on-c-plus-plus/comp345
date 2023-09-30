#ifndef COMP345_GAMEENGINE_H
#define COMP345_GAMEENGINE_H

#include <string>

/*
GameEngine.cpp/GameEngine.h implement the system that tracks the current state of the game and which methods are supposed to be run in each state. The states form the structure of the setup and the game itself.
*/

// Basic definition to avoid definition order within file.
class GameEngine;
// This is the enum used to define all possible states the engine can be in.
enum possibleStates
{
    start,
    mapLoaded,
    mapValidated,
    playersAdded,
    assignReiforcment,
    issueOrders,
    executeOrders,
    win
};
/*
GameEngine is implemented as a container for a string and a boolean, the string representing the state and the boolean being used as a check as to whether the game has ended.
*/
class GameEngine
{
public:
    explicit GameEngine(const possibleStates &initState);
    // Copy constructor
    GameEngine(const GameEngine &rhs);

    GameEngine &operator=(const GameEngine &rhs);
    // Destructor
    ~GameEngine();

    void transaction(std::string command);
    void setState(possibleStates input);
    void setGameOver(bool input);
    [[nodiscard]] possibleStates getState();
    [[nodiscard]] bool getGameOver();

private:
    possibleStates *state;
    bool *gameOver;
};

#endif // COMP345_GAMEENGINE_H