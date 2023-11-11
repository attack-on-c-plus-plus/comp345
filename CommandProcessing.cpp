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

#include "CommandProcessing.h"

#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>

#include "GameEngine.h"
#include "Orders.h"

/**
 * Commands available to the command processor
 */
std::unique_ptr<std::map<CommandType, std::string>> CommandProcessor::commands = std::make_unique<std::map<CommandType, std::string>>(
        std::map<CommandType, std::string> {
                {CommandType::loadmap,      "loadmap"},
                {CommandType::validatemap,  "validatemap"},
                {CommandType::addplayer,    "addplayer"},
                {CommandType::gamestart,    "gamestart"},
                {CommandType::replay,       "replay"},
                {CommandType::quit,         "quit"},
                {CommandType::deploy,       "deploy"},
                {CommandType::advance,      "advance"},
                {CommandType::bomb,         "bomb"},
                {CommandType::blockade,     "blockade"},
                {CommandType::airlift,      "airlift"},
                {CommandType::negotiate,    "negotiate"}
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
    commands_ = new std::vector(*commandProcessor.commands_);
}

/**
 * Destructor
 */
CommandProcessor::~CommandProcessor() {
    for (const auto c : *commands_)
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
        for (const auto c : *commands_)
        {
            delete c;
        }
        commands_->clear();
        delete commands_;
        commands_ = new std::vector(*commandProcessor.commands_);
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

        c = createCommand(gameEngine, commandStr, parameter);
    }
    return *c;
}

Command *
CommandProcessor::createCommand(GameEngine &gameEngine, std::string &commandStr, const std::string &parameter) {
    Command *c = nullptr;
    std::ranges::transform(commandStr, commandStr.begin(), tolower);
    if (commandStr == commands->at(CommandType::loadmap)) {
        c = new LoadMapCommand{gameEngine, parameter};
    }
    else if (commandStr == commands->at(CommandType::validatemap)) {
        c = new ValidateMapCommand{gameEngine};
    }
    else if (commandStr == commands->at(CommandType::addplayer)) {
        c = new AddPlayerCommand{gameEngine, parameter};
    }
    else if (commandStr == commands->at(CommandType::gamestart)) {
        c = new GameStartCommand{gameEngine};
    }
    else if (commandStr == commands->at(CommandType::replay)) {
        c = new ReplayCommand{gameEngine};
    }
    else if (commandStr == commands->at(CommandType::quit)) {
        c = new QuitCommand{gameEngine};
    }
    else { std::cout << "Invalid command. " << commandStr << std::endl; }
    return c;
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
 * Gets a command
 * @param gameEngine
 * @return
 */
Order &CommandProcessor::getOrder(GameEngine &gameEngine) {
    Order *o = nullptr;

    //    Command *c = &readCommand(gameEngine);
//    saveCommand(*c);
    return *o;
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

FileCommandProcessorAdapter::FileCommandProcessorAdapter(const std::string &filename) {
    fileLineReader_ = new FileLineReader(filename);
}

FileCommandProcessorAdapter::~FileCommandProcessorAdapter() {
    delete fileLineReader_;
}

Command &FileCommandProcessorAdapter::readCommand(GameEngine &gameEngine) {
    Command *c = nullptr;

    if (std::string line; fileLineReader_->readLineFromFile(line)) {
        std::stringstream s{line};

        std::string commandStr;
        s >> commandStr;
        std::string parameter;
        if (s.peek() == ' ') {
            s.get();
            getline(s, parameter);
        }
        c = createCommand(gameEngine, commandStr, parameter);
    }
    return *c;
}

FileLineReader::FileLineReader(const std::string &filename) {
    in_ = new std::ifstream(filename);
}

FileLineReader::~FileLineReader() {
    delete in_;
}

bool FileLineReader::readLineFromFile(std::string &line) const {
    return static_cast<bool>(getline(*in_, line));
}
