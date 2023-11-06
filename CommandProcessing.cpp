// Daniel Soldera
// Carson Senthilkumar
// Joe El-Khoury
// Henri Stephane Carbon
// Haris Mahmood

#include <iostream>
#include <memory>
#include <sstream>
#include "CommandProcessing.h"

/**
 * Commands available to the command processor
 */
std::unique_ptr<std::map<CommandType, std::string>> CommandProcessor::commands = std::make_unique<std::map<CommandType, std::string>>(
    std::map<CommandType, std::string> {
        {CommandType::loadmap, "loadmap"},
        {CommandType::validatemap, "validatemap"},
        {CommandType::addplayer, "addplayer"},
        {CommandType::assignterritories, "assignterritories"},
        {CommandType::issueorder, "issueorder"},
        {CommandType::endissueorders, "endissueorders"},
        {CommandType::execorder, "execorder"},
        {CommandType::endexecorders, "endexecorders"},
        {CommandType::win, "win"},
        {CommandType::play, "play"},
        {CommandType::end, "end"}
    });

/**
 * Constructor
 */
CommandProcessor::CommandProcessor() {
    commands_ = new std::vector<Command *>();
}

/**
 * Copy constructor
 * @param commandProcessor
 */
CommandProcessor::CommandProcessor(const CommandProcessor &commandProcessor) :
    Subject(commandProcessor) {
    commands_ = new std::vector<Command *>(*commandProcessor.commands_);
}

/**
 * Destructor
 */
CommandProcessor::~CommandProcessor() {
    for (auto c : *commands_)
    {
        delete c;
    }
    commands_->clear();
    delete commands_;
}

/**
 * Operator= overload
 * @param commandProcessor
 * @return
 */
CommandProcessor &CommandProcessor::operator=(const CommandProcessor &commandProcessor) {
    if (this != &commandProcessor)
    {
        // This is used in case we are re-assigning to ourselves i.e. f = f;
        // We need to clean up the current pointers because destructor won't be called.
        for (auto c : *commands_)
        {
            delete c;
        }
        commands_->clear();
        delete commands_;
        commands_ = new std::vector<Command *>(*commandProcessor.commands_);
    }

    return *this;
}

/**
 * Reads a command from the command line
 * @param gameEngine
 * @return the command
 */
Command &CommandProcessor::readCommand(GameEngine &gameEngine) {
    Command *c = nullptr;
    // read the first element to determine what the name of the is;
    while (!c) {
        std::cout << "Enter command" << std::endl;
        std::string commandStr;
        std::cin >> commandStr;
        std::string parameter;
        if (std::cin.peek() == ' ') {
            std::cin.get();
            getline(std::cin, parameter);
        }

        std::transform(commandStr.begin(), commandStr.end(), commandStr.begin(), ::tolower);
        if (commandStr == CommandProcessor::commands->at(CommandType::loadmap)) {
            c = new LoadMapCommand{gameEngine, parameter};
        }
        else if (commandStr == CommandProcessor::commands->at(CommandType::validatemap)) {
            c = new ValidateMapCommand{gameEngine};
        }
        else if (commandStr == CommandProcessor::commands->at(CommandType::addplayer)) {
            c = new AddPlayerCommand{gameEngine, parameter};
        }
        else if (commandStr == CommandProcessor::commands->at(CommandType::assignterritories)) {
            c = new AssignTerritoriesCommand{gameEngine};
        }
        else if (commandStr == CommandProcessor::commands->at(CommandType::issueorder)) {
            c = new IssueOrdersCommand{gameEngine};
        }
        else if (commandStr == CommandProcessor::commands->at(CommandType::endissueorders)) {
            c = new EndIssueOrdersCommand{gameEngine};
        }
        else if (commandStr == CommandProcessor::commands->at(CommandType::execorder)) {
            c = new ExecuteOrdersCommand{gameEngine};
        }
        else if (commandStr == CommandProcessor::commands->at(CommandType::endexecorders)) {
            c = new EndExecuteOrdersCommand{gameEngine};
        }
        else if (commandStr == CommandProcessor::commands->at(CommandType::win)) {
            c = new WinCommand{gameEngine};
        }
        else if (commandStr == CommandProcessor::commands->at(CommandType::play)) {
            c = new PlayCommand{gameEngine};
        }
        else if (commandStr == CommandProcessor::commands->at(CommandType::end)) {
            c = new QuitCommand{gameEngine};
        }
        else { std::cout << "Invalid command. " << commandStr << std::endl; }
    }
    return *c;
}

/**
 * Gets a command
 * @param gameEngine
 * @return
 */
Command &CommandProcessor::getCommand(GameEngine &gameEngine) {
    Command *c = &readCommand(gameEngine);
    saveCommand(*c);
    return *c;
}

/**
 * Validates a command
 * @param command
 * @return
 */
bool CommandProcessor::validate(Command &command) const {
    return command.validate();
}

/**
 * Saves a command to the command list
 * @param command
 */
void CommandProcessor::saveCommand(Command &command) {
    commands_->push_back(&command);
    Notify(*this);
}

/**
 * Creates a string to log
 * @return
 */
std::string CommandProcessor::stringToLog() const {
    std::stringstream s;

    if (!commands_->empty()) {
        s << "| Command: " << commands_->back()->description();
    }
    return s.str();
}
