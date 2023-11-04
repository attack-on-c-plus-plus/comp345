// Daniel Soldera
// Carson Senthilkumar
// Joe El-Khoury
// Henri Stephane Carbon
// Haris Mahmood

#include <iostream>
#include <memory>
#include "CommandProcessing.h"

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

CommandProcessor::CommandProcessor() {
    commands_ = new std::vector<Command *>();
}

CommandProcessor::CommandProcessor(const CommandProcessor &commandProcessor) {
    commands_ = new std::vector<Command *>(*commandProcessor.commands_);
}

CommandProcessor::~CommandProcessor() {
    for (auto c : *commands_)
    {
        delete c;
    }
    commands_->clear();
    delete commands_;
}

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

Command &CommandProcessor::getCommand(GameEngine &gameEngine) {
    Command *c = &readCommand(gameEngine);

    saveCommand(*c);
    return *c;
}

bool CommandProcessor::validate(Command &command) const {
    return command.validate();
}

void CommandProcessor::saveCommand(Command &command) {
    commands_->push_back(&command);
}

std::string CommandProcessor::stringToLog() const {
    return {};
}
