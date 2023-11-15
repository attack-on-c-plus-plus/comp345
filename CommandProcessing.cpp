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

#include "Cards.h"
#include "GameEngine.h"
#include "Map.h"
#include "Orders.h"
#include "Player.h"

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
                {CommandType::quit,         "quit"}
        });

std::unique_ptr<std::map<OrderType, std::string>> CommandProcessor::orders = std::make_unique<std::map<OrderType, std::string>>(
    std::map<OrderType, std::string> {
        {OrderType::deploy,     "deploy"},
        {OrderType::advance,    "advance"},
        {OrderType::bomb,       "bomb"},
        {OrderType::blockade,   "blockade"},
        {OrderType::airlift,    "airlift"},
        {OrderType::negotiate,  "negotiate"},
        {OrderType::end,        "end"}
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
    Command *command = nullptr;
    while (!command) {
        std::cout << "Enter command (current game state: " << gameEngine << ")" << std::endl;
        std::string commandStr;
        std::cin >> commandStr;
        std::string parameter;
        if (std::cin.peek() == ' ') {
            std::cin.get();
            getline(std::cin, parameter);
        }
        command = createCommand(gameEngine, commandStr, parameter);
        if (!command) std::cout << "Invalid command" << std::endl;
    }
    return *command;
}

Order* CommandProcessor::readOrder(GameEngine& gameEngine, Player &player) {
    Order *order = nullptr;

    while (!order) {
        std::cout << player << " Enter order (type \"end\" when done) ";
        if (gameEngine.state() == GameState::assignReinforcements)
            std::cout << "(available reinforcements: " << player.reinforcementPool() << ")";
        if (gameEngine.state() == GameState::issueOrders)
            std::cout << "(cards available: " << player.getHand() << ")";
        std::cout << std::endl;
        std::string commandStr;
        std::cin >> commandStr;
        std::string parameters;
        if (std::cin.peek() == ' ') {
            std::cin.get();
            getline(std::cin, parameters);
        }
        std::stringstream parameterStream{parameters};
        std::ranges::transform(commandStr, commandStr.begin(), tolower);
        if (commandStr == orders->at(OrderType::end)) break;
        order = createOrder(gameEngine, player, commandStr, parameterStream);
        if (!order) std::cout << "Invalid order" << std::endl;
    }
    return order;
}

Command *
CommandProcessor::createCommand(GameEngine &gameEngine, std::string &commandStr, const std::string &parameter) {
    Command *command = nullptr;
    std::ranges::transform(commandStr, commandStr.begin(), tolower);
    if (commandStr == commands->at(CommandType::loadmap)) {
        command = new LoadMapCommand{gameEngine, parameter};
    }
    else if (commandStr == commands->at(CommandType::validatemap)) {
        command = new ValidateMapCommand{gameEngine};
    }
    else if (commandStr == commands->at(CommandType::addplayer)) {
        command = new AddPlayerCommand{gameEngine, parameter};
    }
    else if (commandStr == commands->at(CommandType::gamestart)) {
        command = new GameStartCommand{gameEngine};
    }
    else if (commandStr == commands->at(CommandType::replay)) {
        command = new ReplayCommand{gameEngine};
    }
    else if (commandStr == commands->at(CommandType::quit)) {
        command = new QuitCommand{gameEngine};
    }
    else { std::cout << "Invalid command. " << commandStr << std::endl; }
    return command;
}

Order *CommandProcessor::createOrder(GameEngine& gameEngine, Player &player, const std::string& orderStr,
    std::istream &parameters) {
    Order *order = nullptr;
    if (orderStr == orders->at(OrderType::deploy)) {
        unsigned territoryId, armies;
        parameters >> territoryId;
        parameters >> armies;
        order = new DeployOrder(gameEngine, player, gameEngine.map().territory(territoryId), armies);
    }
    else if (orderStr == orders->at(OrderType::advance))
    {
        unsigned targetTerritoriesId,sourceTerritoriesId, armies;
        parameters >> sourceTerritoriesId;
        parameters >> targetTerritoriesId;
        parameters >> armies;
        order = new AdvanceOrder(gameEngine,player,gameEngine.map().territory(sourceTerritoriesId),gameEngine.map().territory(targetTerritoriesId),armies);
    }
    else if (orderStr == orders->at(OrderType::airlift))
    {
        unsigned targetTerritoriesId,sourceTerritoriesId, armies;
        parameters >> sourceTerritoriesId;
        parameters >> targetTerritoriesId;
        parameters >> armies;
        order = new AirliftOrder(gameEngine,player,gameEngine.map().territory(sourceTerritoriesId),gameEngine.map().territory(targetTerritoriesId),armies);
    }

    else if (orderStr == orders->at(OrderType::blockade))
    {
        unsigned targetTerritoriesId;
        parameters >> targetTerritoriesId;
        order = new BlockadeOrder(gameEngine,player,gameEngine.map().territory(targetTerritoriesId));
    }
    else if (orderStr == orders->at(OrderType::bomb)) {
        unsigned targetTerritoriesId;
        parameters >> targetTerritoriesId;
        order = new BombOrder(gameEngine,player,gameEngine.map().territory(targetTerritoriesId));
    }
    else if (orderStr == orders->at(OrderType::negotiate)) {
        unsigned otherPlayerId;
        parameters >> otherPlayerId;
        order = new NegotiateOrder(gameEngine,player,*gameEngine.getPlayers().at(otherPlayerId));
    }
    else { std::cout << "Invalid command. " << orderStr << std::endl; }
    return order;
}

/**
 * Gets a command
 * @param gameEngine
 * @return
 */
Command &CommandProcessor::getCommand(GameEngine &gameEngine) {
    Command *command = &readCommand(gameEngine);
    saveCommand(*command);
    return *command;
}

/**
 * Gets a command
 * @param gameEngine
 * @param player
 * @return
 */
Order *CommandProcessor::getOrder(GameEngine &gameEngine, Player &player) {
    return readOrder(gameEngine, player);

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
