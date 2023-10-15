#ifndef COMP345_GAMEENGINE_H
#define COMP345_GAMEENGINE_H

#include <string>

// Daniel Soldera
// Carson Senthilkumar
// Joe El-Khoury
// Henri Stephane Carbon
// Haris Mahmood

/*
 * GameEngine.cpp/GameEngine.h implement the system that tracks the current
 * state of the game and which methods are supposed to be run in each state.
 * The states form the structure of the setup and the game itself.
*/

// Forward declaration.
class GameEngine;
class Command;

// This is the enum used to define all possible states the engine can be in.
enum class GameState
{
    start,
    mapLoaded,
    mapValidated,
    playersAdded,
    assignReinforcements,
    issueOrders,
    executeOrders,
    win,
    gameOver
};

/**
 * GameEngine is implemented as a container for a string and a boolean,
 * the string representing the state and the boolean being used as a
 * check as to whether the game has ended.
 */
class GameEngine
{
public:
    explicit GameEngine(const GameState &gameStates);
    // Copy constructor
    GameEngine(const GameEngine &gameEngine);
    GameEngine &operator=(const GameEngine &gameEngine);
    // Destructor
    ~GameEngine();
    // transaction going to be the object's main method that handles state transitions based on a command given to it.
    void gameLoop();
    [[nodiscard]] GameState state() const;
    [[nodiscard]] bool gameOver() const;
    void startup();
    void play();
    Command *readCommand();
private:
    GameState *state_;
    friend std::ostream &operator<<(std::ostream &os, GameEngine &gameEngine);
};

std::ostream &operator<<(std::ostream &os, GameEngine &gameEngine);

class Command {
public:
    explicit Command(GameEngine &gameEngine);
    Command(const Command &command);
    virtual ~Command();
    virtual bool valid() = 0;
    virtual GameState execute() = 0;
    [[nodiscard]] virtual Command* clone() const = 0;
    Command &operator=(const Command& command);
protected:
    GameEngine *gameEngine_;
};

class LoadMapCommand : public Command {
public:
    explicit LoadMapCommand(GameEngine &gameEngine);
    LoadMapCommand(const LoadMapCommand& loadMap);
    ~LoadMapCommand() override;
    bool valid() override;
    GameState execute() override;
    [[nodiscard]] LoadMapCommand* clone() const override;
    LoadMapCommand &operator=(const LoadMapCommand& command);
};

class ValidateMapCommand : public Command {
public:
    explicit ValidateMapCommand(GameEngine &gameEngine);
    ValidateMapCommand(const ValidateMapCommand &validateMap);
    ~ValidateMapCommand() override;
    bool valid() override;
    GameState execute() override;
    [[nodiscard]] ValidateMapCommand* clone() const override;
    ValidateMapCommand &operator=(const ValidateMapCommand& command);
};

class AddPlayerCommand : public Command {
public:
    explicit AddPlayerCommand(GameEngine &gameEngine);
    AddPlayerCommand(const AddPlayerCommand &addPlayer);
    ~AddPlayerCommand() override;
    bool valid() override;
    GameState execute() override;
    [[nodiscard]] AddPlayerCommand* clone() const override;
    AddPlayerCommand &operator=(const AddPlayerCommand& command);
};

class AssignTerritoriesCommand : public Command {
public:
    explicit AssignTerritoriesCommand(GameEngine &gameEngine);
    AssignTerritoriesCommand(const AssignTerritoriesCommand &assignTerritories);
    ~AssignTerritoriesCommand() override;
    bool valid() override;
    GameState execute() override;
    [[nodiscard]] AssignTerritoriesCommand* clone() const override;
    AssignTerritoriesCommand &operator=(const AssignTerritoriesCommand& command);
};
class IssueOrdersCommand : public Command {
public:
    explicit IssueOrdersCommand(GameEngine &gameEngine);
    IssueOrdersCommand(const IssueOrdersCommand &issueOrders);
    ~IssueOrdersCommand() override;
    bool valid() override;
    GameState execute() override;
    [[nodiscard]] IssueOrdersCommand* clone() const override;
    IssueOrdersCommand &operator=(const IssueOrdersCommand& command);
};
class EndIssueOrdersCommand : public Command {
public:
    explicit EndIssueOrdersCommand(GameEngine &gameEngine);
    EndIssueOrdersCommand(const EndIssueOrdersCommand &endIssueOrders);
    ~EndIssueOrdersCommand() override;
    bool valid() override;
    GameState execute() override;
    [[nodiscard]] EndIssueOrdersCommand* clone() const override;
    EndIssueOrdersCommand &operator=(const EndIssueOrdersCommand& command);
};
class ExecuteOrdersCommand : public Command {
public:
    explicit ExecuteOrdersCommand(GameEngine &gameEngine);
    ExecuteOrdersCommand(const ExecuteOrdersCommand &executeOrders);
    ~ExecuteOrdersCommand() override;
    bool valid() override;
    GameState execute() override;
    [[nodiscard]] ExecuteOrdersCommand* clone() const override;
    ExecuteOrdersCommand &operator=(const ExecuteOrdersCommand& command);
};

class EndExecuteOrdersCommand : public Command {
public:
    explicit EndExecuteOrdersCommand(GameEngine &gameEngine);
    EndExecuteOrdersCommand(const EndExecuteOrdersCommand & endExecuteOrders);
    ~EndExecuteOrdersCommand() override;
    bool valid() override;
    GameState execute() override;
    [[nodiscard]] EndExecuteOrdersCommand* clone() const override;
    EndExecuteOrdersCommand &operator=(const EndExecuteOrdersCommand& command);
};

class WinCommand : public Command {
public:
    explicit WinCommand(GameEngine &gameEngine);
    WinCommand(const WinCommand &win);
    ~WinCommand() override;
    bool valid() override;
    GameState execute() override;
    [[nodiscard]] WinCommand* clone() const override;
    WinCommand &operator=(const WinCommand& command);
};

class PlayCommand : public Command {
public:
    explicit PlayCommand(GameEngine &gameEngine);
    PlayCommand(const PlayCommand &play);
    ~PlayCommand() override;
    bool valid() override;
    GameState execute() override;
    [[nodiscard]] PlayCommand* clone() const override;
    PlayCommand &operator=(const PlayCommand& command);
};
class QuitCommand : public Command {
public:
    explicit QuitCommand(GameEngine &gameEngine);
    QuitCommand(const QuitCommand &quit);
    ~QuitCommand() override;
    bool valid() override;
    GameState execute() override;
    [[nodiscard]] QuitCommand* clone() const override;
    QuitCommand &operator=(const QuitCommand& command);
};
#endif // COMP345_GAMEENGINE_H