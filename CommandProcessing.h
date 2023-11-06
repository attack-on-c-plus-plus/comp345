// Daniel Soldera
// Carson Senthilkumar
// Joe El-Khoury
// Henri Stephane Carbon
// Haris Mahmood

#ifndef COMP345_COMMANDPROCESSING_H
#define COMP345_COMMANDPROCESSING_H

#include <map>
#include <memory>
#include "LoggingObserver.h"
#include "GameEngine.h"

class Command;
enum class GameState;
class GameEngine;

/**
 * Command types that are available
 */
enum class CommandType {
    loadmap,
    validatemap,
    addplayer,
    assignterritories,
    replay,
    quit
};

/**
 * Processes Command from the console
 */
class CommandProcessor : public ILoggable, public Subject {
public:
    CommandProcessor();
    CommandProcessor(const CommandProcessor& commandProcessor);
    CommandProcessor &operator=(const CommandProcessor &gameEngine);
    // Destructor
    ~CommandProcessor();
    Command &getCommand(GameEngine &gameEngine);
    [[nodiscard]] bool validate(Command &command) const;
    [[nodiscard]] std::string stringToLog() const override;
    static std::unique_ptr<std::map<CommandType, std::string>> commands;
protected:
    [[nodiscard]] virtual Command &readCommand(GameEngine &gameEngine);
private:
    void saveCommand(Command &command);
    std::vector<Command *> *commands_;
};
#endif //COMP345_COMMANDPROCESSING_H
