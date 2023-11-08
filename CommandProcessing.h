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
class FileLineReader;

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

/**
 * Processes Command from the console
 */
class CommandProcessor : public ILoggable, public Subject {
public:
    CommandProcessor();
    CommandProcessor(const CommandProcessor& commandProcessor);
    CommandProcessor &operator=(const CommandProcessor &gameEngine);
    // Destructor
    virtual ~CommandProcessor();
    Command &getCommand(GameEngine &gameEngine);
    [[nodiscard]] bool validate(Command &command) const;
    [[nodiscard]] std::string stringToLog() const override;
    static std::unique_ptr<std::map<CommandType, std::string>> commands;
protected:
    [[nodiscard]] virtual Command &readCommand(GameEngine &gameEngine);
    static Command *createCommand(GameEngine &gameEngine, std::string &commandStr, const std::string &parameter);
private:
    void saveCommand(Command &command);
    std::vector<Command *> *commands_;
};

class FileCommandProcessorAdapter : public CommandProcessor {
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
    bool readLineFromFile(std::string &line);
private:
    std::ifstream *in_;
};

#endif //COMP345_COMMANDPROCESSING_H
