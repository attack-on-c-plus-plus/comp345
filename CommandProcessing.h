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

#ifndef COMP345_COMMANDPROCESSING_H
#define COMP345_COMMANDPROCESSING_H

#include <map>
#include <memory>
#include <vector>

#include "LoggingObserver.h"
#include "Player.h"

class Player;
/**
 * Forward declarations
 */
class FileLineReader;
class ILoggable;
class Subject;
class Order;
class Command;
class GameEngine;
class LoadMapCommand;
class ValidateMapCommand;
class AddPlayerCommand;
class GameStartCommand;
class ReplayCommand;
class QuitCommand;

/**
 * Command types that are available
 */
enum class CommandType {
    loadmap,
    validatemap,
    addplayer,
    gamestart,
    replay,
    quit
};

enum class OrderType {
    deploy,
    advance,
    bomb,
    blockade,
    airlift,
    negotiate,
    end
};

/**
 * Processes Command from the console
 */
class CommandProcessor : public ILoggable, public Subject {
public:
    CommandProcessor();
    CommandProcessor(const CommandProcessor& commandProcessor);
    CommandProcessor &operator=(const CommandProcessor &commandProcessor);
    // Destructor
    ~CommandProcessor() override;
    Command &getCommand(GameEngine &gameEngine);
    Order *getOrder(GameEngine &gameEngine, Player &player);
    [[nodiscard]] bool validate(Command &command) const;
    [[nodiscard]] std::string stringToLog() const override;
    static std::unique_ptr<std::map<std::string, CommandType>> commands;
    static std::unique_ptr<std::map<std::string, OrderType>> orders;
    static std::unique_ptr<std::map<std::string, Strategy>> strategies;
protected:
    [[nodiscard]] virtual Command &readCommand(GameEngine &gameEngine);
    [[nodiscard]] virtual Order *readOrder(GameEngine &gameEngine, Player &player);
    static Command *createCommand(GameEngine &gameEngine, std::string &commandStr, std::istream &parameters);
    static Order *createOrder(GameEngine &gameEngine, Player &player, const std::string &orderStr, std::istream &parameters);
private:
    void saveCommand(Command &command);
    std::vector<Command *> *commands_;
};

class FileCommandProcessorAdapter final : public CommandProcessor {
public:
    explicit FileCommandProcessorAdapter(const std::string &filename);
    // don't allow copying or assignment due to file manipulation
    FileCommandProcessorAdapter(const FileCommandProcessorAdapter &fileCommandProcessorAdapter) = delete;
    FileCommandProcessorAdapter &operator=(const FileCommandProcessorAdapter &fileCommandProcessorAdapter) = delete;
    ~FileCommandProcessorAdapter() override;
private:
    Command &readCommand(GameEngine &gameEngine) override;
    FileLineReader *fileLineReader_;
};

class FileLineReader {
public:
    explicit FileLineReader(const std::string &filename);
    // don't allow copying or assignment due to file manipulation
    FileLineReader(const FileLineReader &fileLineReader) = delete;
    FileLineReader &operator=(const FileLineReader &fileLineReader) = delete;
    ~FileLineReader();
    bool readLineFromFile(std::string &line) const;
private:
    std::ifstream *in_;
};

#endif //COMP345_COMMANDPROCESSING_H
