// Daniel Soldera
// Carson Senthilkumar
// Joe El-Khoury
// Henri Stephane Carbon
// Haris Mahmood

#ifndef COMP345_COMMANDPROCESSING_H
#define COMP345_COMMANDPROCESSING_H

#include <map>
#include <memory>
#include <vector>
#include "GameEngine.h"

class FileLineReader;
class Command;
class GameEngine;

/**
 * Command types that are available
 */
enum class CommandType {
    loadmap,
    validatemap,
    addplayer,
    gamestart,
    replay,
    quit,
    deploy,
    advance,
    bomb,
    blockade,
    airlift,
    negotiate
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
    ICommand &getCommand(GameEngine &gameEngine);
    [[nodiscard]] bool validate(Command &command) const;
    [[nodiscard]] std::string stringToLog() const override;
    static std::unique_ptr<std::map<CommandType, std::string>> commands;
protected:
    [[nodiscard]] virtual ICommand &readCommand(GameEngine &gameEngine);
    static ICommand *createCommand(GameEngine &gameEngine, std::string &commandStr, const std::string &parameter);
private:
    void saveCommand(ICommand &command);
    std::vector<ICommand *> *commands_;
};

class FileCommandProcessorAdapter : public CommandProcessor {
public:
    explicit FileCommandProcessorAdapter(const std::string &filename);
    // don't allow copying or assignment due to file manipulation
    FileCommandProcessorAdapter(const FileCommandProcessorAdapter &fileCommandProcessorAdapter) = delete;
    FileCommandProcessorAdapter &operator=(const FileCommandProcessorAdapter &fileCommandProcessorAdapter) = delete;
    ~FileCommandProcessorAdapter() override;
private:
    ICommand &readCommand(GameEngine &gameEngine) override;
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
