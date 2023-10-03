#include "GameEngine.h"
#include <iostream>
using namespace std;

// Main constructor for the GameEngine object
GameEngine::GameEngine(const possibleStates &initState) : state(new possibleStates(initState)) { gameOver = new bool(false); }
// Copy constructor
GameEngine::GameEngine(const GameEngine &rhs) : state(new possibleStates(*rhs.state)) { *gameOver = *rhs.gameOver; }
// Copying over state
GameEngine &GameEngine::operator=(const GameEngine &rhs)
{
    if (this != &rhs)
    { // This is used in case we are re-assigning to ourselves i.e. f = f;
        // We need to clean up the current pointers because destructor won't be called.
        delete state;
        delete gameOver;
        state = new possibleStates(*rhs.state);
        gameOver = new bool(*rhs.gameOver);
    }

    return *this;
}
// Destructor
GameEngine::~GameEngine()
{
    delete state;
    delete gameOver;
};
// Getter for the current game state
possibleStates GameEngine::getState()
{
    return *state;
}
// Getter for the gameOver boolean
bool GameEngine::getGameOver()
{
    return *gameOver;
}
// Setter for the current game state
void GameEngine::setState(possibleStates input)
{
    *state = input;
}
// Setter for the gameOver boolean
void GameEngine::setGameOver(bool input)
{
    *gameOver = input;
}

// This method checks the current state using a switch then uses the command given to change the game's state, display a set message if the state is supposed to remain the same or an error if the command is invalid.
void GameEngine::transaction(string command)
{
    switch (*state)
    {
    case start:
    {
        if (command == "loadmap")
        {
            setState(mapLoaded);
            cout << "Moved to the load map state." << endl;
        }
        else
        {
            cout << "Invalid command, please try again, your current state is start." << endl;
        }
        break;
    }
    case mapLoaded:
    {
        if (command == "loadmap")
        {
            setState(mapLoaded);
            cout << "Loading new map." << endl;
        }
        else
        {
            if (command == "validatemap")
            {
                setState(mapValidated);
                cout << "Moved to map validation state." << endl;
            }
            else
            {
                cout << "Invalid command, please try again, your current state is mapLoaded." << endl;
            }
        }
        break;
    }
    case mapValidated:
    {
        if (command == "addplayer")
        {
            setState(playersAdded);
            cout << "Adding first player." << endl;
        }
        else
        {
            cout << "Invalid command, please try again, your current state is mapValidated." << endl;
        }
        break;
    }
    case playersAdded:
    {
        if (command == "addplayer")
        {
            cout << "Adding additional player." << endl;
        }
        else if (command == "assigncountries")
        {
            setState(assignReiforcment);
            cout << "Assigning countries to players and beggining the game." << endl;
        }
        else
        {
            cout << "Invalid command, please try again, your current state is playersAdded." << endl;
        }
        break;
    }
    case assignReiforcment:
    {
        if (command == "issueorders")
        {
            setState(issueOrders);
            cout << "Moved to issue current player's orders." << endl;
        }
        else
        {
            cout << "Invalid command, please try again, your current state is assignReinforcement." << endl;
        }
        break;
    }
    case issueOrders:
    {
        if (command == "issueorders")
        {
            cout << "Issued orders, moved to next player." << endl;
        }
        else if (command == "endissueorders")
        {
            setState(executeOrders);
            cout << "Moved to execute current player's orders." << endl;
        }
        else
        {
            cout << "Invalid command, please try again, your current state is issueOrders." << endl;
        }
        break;
    }
    case executeOrders:
    {
        if (command == "execorder")
        {
            cout << "Executed orders, moved to next player." << endl;
        }
        else if (command == "endexecorders")
        {
            setState(assignReiforcment);
            cout << "Executed orders, moved to assign reinforcements on the next turn." << endl;
        }
        else if (command == "win")
        {
            setState(win);
            cout << "The current player has won!!! Enter the command 'play' to start a new game or 'end' to close the program." << endl;
        }
        else
        {
            cout << "Invalid command, please try again, your current state is executeOrders." << endl;
        }
        break;
    }
    case win:
    {
        if (command == "play")
        {
            setState(start);
            cout << "Restarting game." << endl;
        }
        else if (command == "end")
        {
            setGameOver(true);
            cout << "Game ended, closing program." << endl;
        }
        else
        {
            cout << "Invalid command, please try again, your current state is win." << endl;
        }
        break;
    }
    }
}