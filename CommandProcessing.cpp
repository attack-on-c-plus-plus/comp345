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

#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <regex>
#include <sstream>

#include "Cards.h"
#include "GameEngine.h"
#include "Map.h"
#include "Orders.h"
#include "Player.h"
#include "PlayerStrategies.h"

/**
 * Commands available to the command processor
 */
std::unique_ptr<std::map<std::string, CommandType>> CommandProcessor::commands = std::make_unique<std::map<std::string, CommandType>>(
        std::map<std::string, CommandType> {
            {"tournament", CommandType::tournament},
            { "loadmap", CommandType::loadmap},
            { "validatemap", CommandType::validatemap},
            { "addplayer", CommandType::addplayer},
            { "gamestart", CommandType::gamestart},
            { "replay", CommandType::replay},
            { "quit", CommandType::quit}
        });

/**
 * Orders available to the command processor
 */
std::unique_ptr<std::map<std::string, OrderType>> CommandProcessor::orders = std::make_unique<std::map<std::string, OrderType>>(
    std::map<std::string, OrderType> {
        {"deploy", OrderType::deploy },
        {"advance", OrderType::advance },
        {"end", OrderType::end }
        });

/**
 * Cards available to the command processor
 */
std::unique_ptr<std::map<std::string, CardType>> CommandProcessor::cards = std::make_unique<std::map<std::string, CardType>>(
    std::map<std::string, CardType> {
        {"bomb", CardType::bomb },
        {"blockade", CardType::blockade },
        {"airlift", CardType::airlift },
        {"negotiate", CardType::diplomacy },
        {"reinforce", CardType::reinforcement }
        });

/**
 * Strategies available to the command processor
 */
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

/**
 * \brief Reads an order
 * \param gameEngine
 * \param player
 */
void CommandProcessor::readOrder(GameEngine&gameEngine, Player&player) {
    bool validOrder = false;
    while (!validOrder) {
        std::cout << player << " Enter order (type \"end\" when done) ";
        if (gameEngine.state() == GameState::issueOrders)
            std::cout << "(cards available: " << player.hand() << ")";
        std::cout << std::endl;
        std::string orderStr;
        std::cin >> orderStr;
        std::ranges::transform(orderStr, orderStr.begin(), tolower);
        if (orders->contains(orderStr)) {
            validOrder = true;
            unsigned targetId, armies;
            switch (orders->at(orderStr)) {
                case OrderType::deploy: {
                    std::cout << "Available deploy territories: " << std::endl;
                    for (const auto t: player.toDefend()) {
                        std::cout << "\t" << *t << " { armies: " << t->armyCount() << " }" << std::endl;
                    }
                    std::cin >> targetId;
                    std::cout << "Armies: (available: " << player.availableReinforcements() << ")" << std::endl;
                    std::cin >> armies;
                    player.orderList().addOrder(DeployOrder(gameEngine, player, gameEngine.map().territory(targetId), armies));
                    break;
                }
                case OrderType::advance: {
                    unsigned sourceId;
                    std::cout << "Available source territories: " << std::endl;
                    for (const auto t: player.territories()) {
                        std::cout << "\t" << *t << " { armies: " << t->armyCount() << " }" << std::endl;
                    }
                    std::cin >> sourceId;
                    Territory &source = gameEngine.map().territory(sourceId);
                    std::cout << "Available target territories: " << std::endl;
                    for (const auto t: gameEngine.map().adjacencies(source)) {
                        std::cout << "\t" << *t << " { owner: " << t->owner() << ", armies " << t->armyCount() << " }" << std::endl;
                    }
                    std::cin >> targetId;
                    std::cout << "Armies: (available: " << source.armyCount() << ")" << std::endl;
                    std::cin >> armies;
                    player.orderList().addOrder(AdvanceOrder(gameEngine, player, source, gameEngine.map().territory(targetId), armies));
                    break;
                }
                case OrderType::end: {
                    player.orderList().addOrder(EndOrder(gameEngine, player));
                    break;
                }
                default:
                    throw std::out_of_range("invalid order");
            }
        } else if (cards->contains(orderStr) && player.hand().has(cards->at(orderStr))) {
            validOrder = true;
            player.play(cards->at(orderStr), std::cin);
        } else {
            std::cout << "Invalid order: " << orderStr << std::endl;
        }

    }

}

/**
 * \brief Creates a command user input
 * \param gameEngine the game engine
 * \param commandStr the command string
 * \param parameters the other parameters supplied
 * \return
 */
Command *
CommandProcessor::createCommand(GameEngine &gameEngine, std::string &commandStr, std::istream &parameters) {
    Command *command = nullptr;
    std::ranges::transform(commandStr, commandStr.begin(), tolower);
    if (!commands->contains(commandStr)) std::cout << "Invalid command. " << commandStr << std::endl;
    else {
        switch (commands->at(commandStr)) {
            case CommandType::tournament: {
                std::string tournamentParams;
                getline(parameters, tournamentParams);

                const std::regex tournament_regex(R"(-M (.+)-P ((?:(?:neutral|cheater|aggressive|benevolent)[[:blank:]])+)-G (\d+) -D (\d+))", std::regex_constants::icase);

                std::vector<std::string> maps;
                std::set<Strategy> players;
                unsigned games = 0;
                unsigned rounds = 0;

                if (std::smatch tournament_match; std::regex_match(tournamentParams, tournament_match, tournament_regex))
                {
                    std::stringstream ss1(tournament_match[1].str());
                    std::string tmp, newPart;
                    while(std::getline(ss1, newPart, ' ')){
                        if (!tmp.empty()) tmp.append(" ");
                        tmp.append(newPart);
                        if (std::filesystem::exists(tmp) && std::filesystem::is_regular_file(tmp)) {
                            maps.push_back(tmp);
                            tmp = "";
                        } else if (tmp.find('.') != std::string::npos) {
                            tmp = "";
                        }
                    }
                    std::stringstream ss2(tournament_match[2].str());
                    while(std::getline(ss2, tmp, ' ')){
                        players.insert(strategies->at(tmp));
                    }
                    games = stoi(tournament_match[3].str());
                    rounds = stoi(tournament_match[4].str());
                }

                command = new TournamentCommand{gameEngine, maps, players, games, rounds};
                break;
            }
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

/**
 * \brief Creates an irder from user input
 * \param gameEngine the game engine
 * \param player the player
 * \param orderStr the order string
 * \param parameters the other parameters supplied
 * \return
 */
Order *CommandProcessor::createOrder(const GameEngine& gameEngine, Player &player, const std::string& orderStr,
                                     std::istream &parameters) {
    Order* order;
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
        case OrderType::end:
            order = new EndOrder(gameEngine, player);
            break;
        default:
            throw std::out_of_range("invalid order");
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
void CommandProcessor::getOrder(GameEngine&gameEngine, Player&player) {
    readOrder(gameEngine, player);

}

/**
 * Validates a command
 * @param command
 * @return
 */
bool CommandProcessor::validate(Command &command) {
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

/**
 * \brief Constructor
 * \param filename
 */
FileCommandProcessorAdapter::FileCommandProcessorAdapter(const std::string &filename) {
    fileLineReader_ = new FileLineReader(filename);
}

/**
 * \brief Destructor
 */
FileCommandProcessorAdapter::~FileCommandProcessorAdapter() {
    delete fileLineReader_;
}

/**
 * \brief Read a command
 * \param gameEngine the game engine
 * \return a command
 */
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

/**
 * \brief Constructor
 * \param filename
 */
FileLineReader::FileLineReader(const std::string &filename) {
    in_ = new std::ifstream(filename);
}

/**
 * \brief Destructor
 */
FileLineReader::~FileLineReader() {
    delete in_;
}

/**
 * \brief Reads a line from a file
 * \param line the line read
 * \return true if a line was read; false otherwise
 */
bool FileLineReader::readLineFromFile(std::string &line) const {
    return static_cast<bool>(getline(*in_, line));
}
