#ifndef COMP345_GAMEENGINE_H
#define COMP345_GAMEENGINE_H

#include <string>

#include "LoggingObserver.h"
#include "Map.h"
#include "CommandProcessing.h"

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
class CommandProcessor;
class Player;
class Map;

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

std::ostream &operator<<(std::ostream &os, GameState state);

/**
 * GameEngine is implemented as a container for a string and a boolean,
 * the string representing the state and the boolean being used as a
 * check as to whether the game has ended.
 */
class GameEngine : public ILoggable, public Subject {
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
    [[nodiscard]] Map &map() const;
    [[nodiscard]] std::vector<Player *>& getPlayers();
    void map(Map &map);
    void startup();
    void play();
    void executeOrdersPhase();
    [[nodiscard]] std::string stringToLog() const override;
    void transition(GameState gameState);
private:
    GameState *state_;
    friend std::ostream &operator<<(std::ostream &os, const GameEngine &gameEngine);
    Map *map_;
    std::vector<Player *> *players_;
    void removeEliminatedPlayers();
    void checkWinningCondition();
    CommandProcessor *commandProcessor_;
};

class Command : public ILoggable, public Subject {
public:
    explicit Command(GameEngine &gameEngine, const std::string &description);
    Command(const Command &command);
    virtual ~Command();
    virtual bool validate();
    virtual GameState execute() = 0;
    [[nodiscard]] virtual Command* clone() const = 0;
    void saveEffect(const std::string &effect);
    std::string &description() const;
    [[nodiscard]] std::string stringToLog() const override;
    Command &operator=(const Command& command);
protected:
    GameEngine *gameEngine_;
    std::string *description_;
    std::string *effect_;
private:
    friend std::ostream &operator<<(std::ostream &os, const Command &command);
};

class LoadMapCommand : public Command {
public:
    explicit LoadMapCommand(GameEngine &gameEngine, const std::string &filename);
    LoadMapCommand(const LoadMapCommand& loadMap);
    ~LoadMapCommand() override;
    bool validate() override;
    GameState execute() override;
    [[nodiscard]] LoadMapCommand* clone() const override;
    LoadMapCommand &operator=(const LoadMapCommand& command);
private:
    std::string *filename_;
};

class ValidateMapCommand : public Command {
public:
    explicit ValidateMapCommand(GameEngine &gameEngine);
    ValidateMapCommand(const ValidateMapCommand &validateMap);
    ~ValidateMapCommand() override;
    bool validate() override;
    GameState execute() override;
    [[nodiscard]] ValidateMapCommand* clone() const override;
    ValidateMapCommand &operator=(const ValidateMapCommand& command);
};

class AddPlayerCommand : public Command {
public:
    explicit AddPlayerCommand(GameEngine &gameEngine, const std::string &playerName);
    AddPlayerCommand(const AddPlayerCommand &addPlayer);
    ~AddPlayerCommand() override;
    bool validate() override;
    GameState execute() override;
    [[nodiscard]] AddPlayerCommand* clone() const override;
    AddPlayerCommand &operator=(const AddPlayerCommand& command);
private:
    std::string *playerName_;
};

class AssignTerritoriesCommand : public Command {
public:
    explicit AssignTerritoriesCommand(GameEngine &gameEngine);
    AssignTerritoriesCommand(const AssignTerritoriesCommand &assignTerritories);
    ~AssignTerritoriesCommand() override;
    bool validate() override;
    GameState execute() override;
    [[nodiscard]] AssignTerritoriesCommand* clone() const override;
    AssignTerritoriesCommand &operator=(const AssignTerritoriesCommand& command);
};
class ReplayCommand : public Command {
public:
    explicit ReplayCommand(GameEngine &gameEngine);
    ReplayCommand(const ReplayCommand &play);
    ~ReplayCommand() override;
    bool validate() override;
    GameState execute() override;
    [[nodiscard]] ReplayCommand* clone() const override;
    ReplayCommand &operator=(const ReplayCommand& command);
};
class QuitCommand : public Command {
public:
    explicit QuitCommand(GameEngine &gameEngine);
    QuitCommand(const QuitCommand &quit);
    ~QuitCommand() override;
    bool validate() override;
    GameState execute() override;
    [[nodiscard]] QuitCommand* clone() const override;
    QuitCommand &operator=(const QuitCommand& command);
};
#endif // COMP345_GAMEENGINE_H