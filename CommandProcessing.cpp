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
std::unique_ptr<std::map<std::string, CommandType>> CommandProcessor::commands = std::make_unique<std::map<std::string, CommandType>>(
        std::map<std::string, CommandType> {
                { "loadmap", CommandType::loadmap},
                { "validatemap", CommandType::validatemap},
                { "addplayer", CommandType::addplayer},
                { "gamestart", CommandType::gamestart},
                { "replay", CommandType::replay},
                { "quit", CommandType::quit}
        });

std::unique_ptr<std::map<std::string, OrderType>> CommandProcessor::orders = std::make_unique<std::map<std::string, OrderType>>(
    std::map<std::string, OrderType> {
        {"deploy", OrderType::deploy },
        {"advance", OrderType::advance },
        {"bomb", OrderType::bomb },
        {"blockade", OrderType::blockade },
        {"airlift", OrderType::airlift },
        {"negotiate", OrderType::negotiate },
        {"end", OrderType::end }
        });

std::unique_ptr<std::map<std::string, Strategy>> CommandProcessor::strategies = std::make_unique<std::map<std::string, Strategy>>(
        std::map<std::string, Strategy> {
            {"neutral",     Strategy::Neutral    },
            {"cheater",     Strategy::Cheater    },
            {"human",       Strategy::Human      },
            {"aggressive",  Strategy::Aggressive },
            {"benevolent",  Strategy::Benevolent  }
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
        std::string parameters;
        if (std::cin.peek() == ' ') {
            std::cin.get();
            getline(std::cin, parameters);
        }
        std::stringstream parameterStream{parameters};
        command = createCommand(gameEngine, commandStr, parameterStream);
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
            std::cout << "(cards available: " << player.hand() << ")";
        std::cout << std::endl;
        std::string orderStr;
        std::cin >> orderStr;
        std::string parameters;
        if (std::cin.peek() == ' ') {
            std::cin.get();
            getline(std::cin, parameters);
        }
        std::stringstream parameterStream{parameters};
        std::ranges::transform(orderStr, orderStr.begin(), tolower);
        if (!orders->contains(orderStr)) {std::cout << "Invalid order. " << orderStr << std::endl; continue;}
        if (orders->at(orderStr) == OrderType::end) break;
        order = createOrder(gameEngine, player, orderStr, parameterStream);
        if (!order) std::cout << "Invalid order" << std::endl;
    }
    return order;
}

Command *
CommandProcessor::createCommand(GameEngine &gameEngine, std::string &commandStr, std::istream &parameters) {
    Command *command = nullptr;
    std::ranges::transform(commandStr, commandStr.begin(), tolower);
    if (!commands->contains(commandStr)) std::cout << "Invalid command. " << commandStr << std::endl;
    else {
        switch (commands->at(commandStr)) {
            case CommandType::loadmap: {
                std::string filename;
                parameters >> filename;
                command = new LoadMapCommand{gameEngine, filename};
                break;
            }
            case CommandType::validatemap:
                command = new ValidateMapCommand{gameEngine};
                break;
            case CommandType::addplayer: {
                std::string strategy;
                parameters >> strategy;
                std::string name;
                if (parameters.peek() == ' ') {
                    parameters.get();
                    getline(parameters, name);
                }
                getline(parameters, name);
                command = new AddPlayerCommand{gameEngine, name, strategies->at(strategy)};
                break;
            }
            case CommandType::gamestart:
                command = new GameStartCommand{gameEngine};
                break;
            case CommandType::replay:
                command = new ReplayCommand{gameEngine};
                break;
            case CommandType::quit:
                command = new QuitCommand{gameEngine};
                break;
        }
    }
    return command;
}

Order *CommandProcessor::createOrder(GameEngine& gameEngine, Player &player, const std::string& orderStr,
    std::istream &parameters) {
    Order *order = nullptr;
    unsigned targetTerritoriesId, sourceTerritoriesId, armies;
    switch (orders->at(orderStr)) {
        case OrderType::deploy:
            parameters >> targetTerritoriesId;
            parameters >> armies;
            order = new DeployOrder(gameEngine, player, gameEngine.map().territory(targetTerritoriesId), armies);
            break;
        case OrderType::advance:
            parameters >> sourceTerritoriesId;
            parameters >> targetTerritoriesId;
            parameters >> armies;
            order = new AdvanceOrder(gameEngine, player, gameEngine.map().territory(sourceTerritoriesId),
                                     gameEngine.map().territory(targetTerritoriesId), armies);
            break;
        case OrderType::bomb:
            parameters >> targetTerritoriesId;
            order = new BombOrder(gameEngine, player, gameEngine.map().territory(targetTerritoriesId));
            break;
        case OrderType::blockade:
            parameters >> targetTerritoriesId;
            order = new BlockadeOrder(gameEngine, player, gameEngine.map().territory(targetTerritoriesId));
            break;
        case OrderType::airlift:
            parameters >> sourceTerritoriesId;
            parameters >> targetTerritoriesId;
            parameters >> armies;
            order = new AirliftOrder(gameEngine, player, gameEngine.map().territory(sourceTerritoriesId),
                                     gameEngine.map().territory(targetTerritoriesId), armies);
            break;
        case OrderType::negotiate:
            unsigned otherPlayerId;
            parameters >> otherPlayerId;
            order = new NegotiateOrder(gameEngine, player, *gameEngine.getPlayers().at(otherPlayerId));
            break;
        case OrderType::end:
            break;

    }
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
        std::string parameters;
        if (s.peek() == ' ') {
            s.get();
            getline(s, parameters);
        }
        std::stringstream parameterStream{parameters};
        c = createCommand(gameEngine, commandStr, parameterStream);
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
