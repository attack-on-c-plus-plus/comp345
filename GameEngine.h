/**
 ************************************
 * COMP 345 Professor Hakim Mellah
 ************************************
 * @author Team 5 Attack on C++
 * @author Daniel Soldera
 * @author Carson Senthilkumar
 * @author Joe El-Khoury
 * @author Henri Stephane Carbon
 * @author Haris Mahmood
 */

#ifndef COMP345_GAMEENGINE_H
#define COMP345_GAMEENGINE_H

#include <random>
#include <string>

#include "LoggingObserver.h"

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

class Players;
enum class Strategy;
class IRandom;
// Forward declaration.
class CommandProcessor;
class Player;
class Map;
class Deck;
class Territory;

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
class GameEngine final : public ILoggable, public Subject
{
public:
    explicit GameEngine(CommandProcessor &commandProcessor, const IRandom &random);
    // Copy constructor
    GameEngine(const GameEngine &gameEngine) = delete;
    GameEngine &operator=(const GameEngine &gameEngine) = delete;
    // Destructor
    ~GameEngine() override;
    // transaction going to be the object's main method that handles state transitions based on a command given to it.
    void gameLoop();
    [[nodiscard]] GameState state() const;
    [[nodiscard]] GameState previousState() const;
    [[nodiscard]] bool gameOver() const;
    [[nodiscard]] Map &map() const;
    [[nodiscard]] Players& players() const;
    [[nodiscard]] Deck &deck() const;
    void map(Map &map);
    void startup();
    void mainGameLoop();
    void reinforcementPhase();
    void issuingOrderPhase();
    void executeOrdersPhase() const;
    void gameOverPhase();
    [[nodiscard]] std::string stringToLog() const override;
    void transition(GameState newState);
    [[nodiscard]] const IRandom &random() const;
private:
    GameState *state_;
    GameState *previousState_;
    Map *map_;
    Deck *deck_;
    const IRandom *random_;
    Players *players_;
    CommandProcessor *commandProcessor_;
    void resetGameElements();
    friend std::ostream &operator<<(std::ostream &os, const GameEngine &gameEngine);
};

class Command : public ILoggable, public Subject {
public:
    explicit Command(GameEngine &gameEngine, const std::string &description);
    Command(const Command &command);
    ~Command() override;
    void saveEffect(const std::string &effect);
    [[nodiscard]] virtual bool validate();
    virtual void execute() = 0;
    [[nodiscard]] virtual std::string &description() const;
    [[nodiscard]] std::string stringToLog() const override;
    Command &operator=(const Command &command);
protected:
    GameEngine *gameEngine_;
    std::string *description_;
    std::string *effect_;

private:
    friend std::ostream &operator<<(std::ostream &os, const Command &command);
};

class LoadMapCommand final : public Command
{
public:
    explicit LoadMapCommand(GameEngine &gameEngine, const std::string &filename);
    LoadMapCommand(const LoadMapCommand &loadMap);
    ~LoadMapCommand() override;
    bool validate() override;
    void execute() override;
    LoadMapCommand &operator=(const LoadMapCommand &command);
private:
    std::string *filename_;
};

class ValidateMapCommand final : public Command
{
public:
    explicit ValidateMapCommand(GameEngine &gameEngine);
    ValidateMapCommand(const ValidateMapCommand &validateMap);
    ~ValidateMapCommand() override;
    bool validate() override;
    void execute() override;
    ValidateMapCommand &operator=(const ValidateMapCommand &command);
};

class AddPlayerCommand final : public Command
{
public:
    explicit AddPlayerCommand(GameEngine &gameEngine, const std::string &playerName, const Strategy strategy);
    AddPlayerCommand(const AddPlayerCommand &addPlayer);
    ~AddPlayerCommand() override;
    bool validate() override;
    void execute() override;
    AddPlayerCommand &operator=(const AddPlayerCommand &command);

private:
    std::string *playerName_;
    const Strategy* strategy_;
};

class GameStartCommand final : public Command
{
public:
    explicit GameStartCommand(GameEngine &gameEngine);
    GameStartCommand(const GameStartCommand &assignTerritories);
    ~GameStartCommand() override;
    bool validate() override;
    void execute() override;
    GameStartCommand &operator=(const GameStartCommand &command);
    void assignTerritories(std::vector<Territory *> &territories, std::ostream &os) const;
    void setPlayerTurnOrder(std::ostream &os) const;
};
class ReplayCommand final : public Command
{
public:
    explicit ReplayCommand(GameEngine &gameEngine);
    ReplayCommand(const ReplayCommand &play);
    ~ReplayCommand() override;
    bool validate() override;
    void execute() override;
    ReplayCommand &operator=(const ReplayCommand &command);
};
class QuitCommand final : public Command
{
public:
    explicit QuitCommand(GameEngine &gameEngine);
    QuitCommand(const QuitCommand &quit);
    ~QuitCommand() override;
    bool validate() override;
    void execute() override;
    QuitCommand &operator=(const QuitCommand &command);
};

class IRandom {
public:
    virtual ~IRandom() = default;
    [[nodiscard]] virtual unsigned generate(unsigned from, unsigned to) const = 0;
};

class Random final : public IRandom {
public:
    Random();
    Random(const Random& random) = delete;
    ~Random() override;
    Random &operator=(const Random &command) = delete;
    [[nodiscard]] unsigned generate(unsigned from, unsigned to) const override;
private:
    std::default_random_engine *eng;
    std::uniform_int_distribution<unsigned> *u;
};

class FakeRandom final : public IRandom {
public:
    FakeRandom();
    FakeRandom(const FakeRandom& random) = delete;
    ~FakeRandom() override;
    FakeRandom &operator=(const FakeRandom &command) = delete;
    [[nodiscard]] unsigned generate(unsigned from, unsigned to) const override;
private:
    std::mt19937 *eng;
    std::uniform_int_distribution<unsigned> *u;
};


#endif // COMP345_GAMEENGINE_H
