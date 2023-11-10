#include "GameEngine.h"
#include "Cards.h"
#include "Player.h"
#include <iostream>
#include <filesystem>
#include <cctype>
#include <algorithm>
#include <sstream>
#include <random>
#include <cstdlib>

// Daniel Soldera
// Carson Senthilkumar
// Joe El-Khoury
// Henri Stephane Carbon
// Haris Mahmood

std::ostream &operator<<(std::ostream &os, GameState state)
{
    switch (state)
    {
    case GameState::start:
        os << "start";
        break;
    case GameState::mapLoaded:
        os << "mapLoaded";
        break;
    case GameState::mapValidated:
        os << "mapValidated";
        break;
    case GameState::playersAdded:
        os << "playersAdded";
        break;
    case GameState::assignReinforcements:
        os << "assignReinforcements";
        break;
    case GameState::issueOrders:
        os << "issueOrders";
        break;
    case GameState::executeOrders:
        os << "executeOrders";
        break;
    case GameState::win:
        os << "win";
        break;
    default:
        os.setstate(std::ios_base::failbit);
    }
    return os;
}

/**
 * Main constructor for the GameEngine object
 * @param commandProcessor
 */
GameEngine::GameEngine(CommandProcessor &commandProcessor) :
    state_{new GameState(GameState::start)},
    map_{new Map} {
    commandProcessor_ = &commandProcessor;
    players_ = new std::vector<Player *>();
    deck_ = new Deck(24);
}

/**
 * Copy constructor
 * @param gameEngine
 */
GameEngine::GameEngine(const GameEngine &gameEngine) :
    Subject(gameEngine), state_{new GameState(*gameEngine.state_)},
    map_{new Map(*gameEngine.map_)} {
    commandProcessor_ = gameEngine.commandProcessor_;
    players_ = new std::vector<Player *>(*gameEngine.players_);
    deck_ = new Deck(*gameEngine.deck_);
}

/**
 * Destructor
 */
GameEngine::~GameEngine()
{
    delete state_;
    delete map_;
    // Delete each object in vector
    for (auto p : *players_)
    {
        delete p;
    }
    players_->clear();
    delete players_;
    delete deck_;
}

/**
 * Operator= overload
 * @param gameEngine
 * @return
 */
GameEngine &GameEngine::operator=(const GameEngine &gameEngine)
{
    if (this != &gameEngine)
    {
        // This is used in case we are re-assigning to ourselves i.e. f = f;
        // We need to clean up the current pointers because destructor won't be called.
        delete state_;
        delete map_;
        for (auto p : *players_)
        {
            delete p;
        }
        players_->clear();
        delete players_;
        delete deck_;
        state_ = new GameState(*gameEngine.state_);
        players_ = new std::vector<Player *>(*gameEngine.players_);
        map_ = new Map(*gameEngine.map_);
        commandProcessor_ = gameEngine.commandProcessor_;
        deck_ = new Deck(*gameEngine.deck_);
    }
    return *this;
}

/**GameEngine *gameEngine_;
 * Gets the current game state
 * @return
 */
GameState GameEngine::state() const
{
    return *state_;
}
/*
 * Gets the current map
 */
Map &GameEngine::map() const
{
    return *map_;
}
/*
 * Sets the map to the gameEngine
 */
void GameEngine::map(Map &map)
{
    map_ = &map;
}
/**
 * Gets the vector containing the players
 */
std::vector<Player *> &GameEngine::getPlayers()
{
    return *players_;
}

/**
 * Gets the Deck
 */
Deck &GameEngine::getDeck() const
{
    return *deck_;
}
/**
 * Determines if the game is over
 * @return true if over; false otherwise
 */
bool GameEngine::gameOver() const
{
    return *state_ == GameState::gameOver;
}

void GameEngine::gameLoop()
{
    while (*state_ != GameState::gameOver)
    {
        std::cout << "Preparing game" << std::endl;
        startup();
        std::cout << "Playing game" << std::endl;
        mainGameLoop();
    }
}

void GameEngine::startup()
{
    ICommand *command;
    resetGameElements();
    while (*state_ != GameState::assignReinforcements) {
        command = &commandProcessor_->getCommand(*this);
        if (command == nullptr) continue;
        command->execute();
    }
}

void GameEngine::resetGameElements() {
    delete map_;
    for (auto p : *players_)
    {
        delete p;
    }
    players_->clear();
    delete players_;
    map_ = new Map();
    players_ = new std::vector<Player *>();
}

void GameEngine::mainGameLoop() {
    while (*state_ != GameState::win) {
        reinforcementPhase();
        issuingOrderPhase();
        executeOrdersPhase();
        removeEliminatedPlayers();
        checkWinningCondition();
    }
    gameOverPhase();
}

void GameEngine::gameOverPhase() {
    ICommand *command;
    while (*state_ == GameState::win)
    {
        command = &commandProcessor_->getCommand(*this);
        command->execute();
    }
}

std::ostream &operator<<(std::ostream &os, const GameEngine &gameEngine)
{
    switch (*gameEngine.state_)
    {
    case GameState::start:
        os << "start";
        break;
    case GameState::mapLoaded:
        os << "map loaded";
        break;
    case GameState::mapValidated:
        os << "map validated";
        break;
    case GameState::playersAdded:
        os << "players added";
        break;
    case GameState::assignReinforcements:
        os << "assign reinforcements";
        break;
    case GameState::issueOrders:
        os << "issue orders";
        break;
    case GameState::executeOrders:
        os << "execute orders";
        break;
    case GameState::win:
        os << "win";
        break;
    default:
        os.setstate(std::ios_base::failbit);
    }
    return os;
}

std::string GameEngine::stringToLog() const
{
    std::stringstream s;
    s << "| Game Engine new state: " << *this;
    if (*state_ == GameState::win)
        s << " player " << players_->at(0) << " wins!";
    return s.str();
}

/**
 * Transitions the Game Engine state to the new state
 * @param gameState
 */
void GameEngine::transition(const GameState newState)
{
    *state_ = newState;
    Notify(*this);
}

/**
 * Executes players orders
 */
void GameEngine::executeOrdersPhase()
{
    if (*state_ != GameState::executeOrders)
        return; // Game engine is in the wrong state

    // execute player orders
    for (auto player : *players_)
    {
        if (player->getTerritories().empty()) // if player has no territories skip turn
            continue;
        player->orderList().executeOrders();
        player->removeNegotiators();
    }
}

/**
 * Check if game is won
 */
void GameEngine::checkWinningCondition()
{ // Only 1 player left the game is over
    if (*state_ != GameState::executeOrders)
        return; // Game engine is in the wrong state
    if (players_->size() == 1)
    {
        transition(GameState::win);
    }

        transition(GameState::assignReinforcements);

}

/**
 * Removes eliminated players
 */
void GameEngine::removeEliminatedPlayers()
{ // remove players which have no territories
    for (auto it = players_->begin(); it != players_->end(); ++it)
    {
        if ((*it)->getTerritories().empty())
        {
            delete *it;
            players_->erase(it);
            --it;
        }
    }
}

/**
 * Executes the reinforcement phase
 */
void GameEngine::reinforcementPhase()
{
    if (*state_ != GameState::assignReinforcements)
        return; // Game engine is in the wrong state
//    // TODO: implement issue #110
//    std::cout << "begin reinforcements" << std::endl;
//    for (auto player : *players_){
//        int territorySize = static_cast<int>(player->getTerritories().size());
//        int reinforcements = territorySize/3;
//
//        //TODO: Insert Country Specific Reinforcements here
//
//        if(reinforcements < 3){ reinforcements = 3;}
//        player->add(reinforcements);
//        std::cout << "added " << reinforcements << " armies for player " << player->getName() << std::endl;
//    }
    transition(GameState::issueOrders);
}

/**
 * Executes the issue orders phase
 */
void GameEngine::issuingOrderPhase()
{
    if (*state_ != GameState::issueOrders)
        return; // Game engine is in the wrong state
    // TODO: implement issue #111
    std::cout << "begin issuing orders" << std::endl;
    transition(GameState::executeOrders);
}

Command::Command(GameEngine &gameEngine, const std::string &description)
{
    gameEngine_ = &gameEngine;
    description_ = new std::string(description);
    effect_ = new std::string();
}

Command::~Command()
{
    delete description_;
    delete effect_;
}

Command::Command(const Command &command) : Subject(command)
{
    gameEngine_ = command.gameEngine_;
    description_ = new std::string(*command.description_);
    effect_ = new std::string(*command.effect_);
}

Command &Command::operator=(const Command &command)
{
    if (this != &command)
    {
        delete description_;
        delete effect_;
        gameEngine_ = command.gameEngine_;
        description_ = new std::string(*command.description_);
        effect_ = new std::string(*command.effect_);
    }
    return *this;
}
std::string &Command::description() const
{
    return *description_;
}

std::string Command::stringToLog() const
{
    std::stringstream s;
    s << "| Command's Effect: " << *effect_;
    return s.str();
}

std::ostream &operator<<(std::ostream &os, const Command &command)
{
    os << *command.effect_;
    return os;
}

void Command::saveEffect(const std::string &effect)
{
    *effect_ = effect;
    std::cout << effect << std::endl;
    Notify(*this);
}

bool Command::validate()
{
    std::stringstream s;
    s << "Command \"" << *description_ << "\" not valid in state \"" << *gameEngine_ << "\".";
    saveEffect(s.str());
    return false;
}

LoadMapCommand::LoadMapCommand(GameEngine &gameEngine, const std::string &filename) : Command(gameEngine, "LoadMap"), filename_{new std::string(filename)} {}

LoadMapCommand::LoadMapCommand(const LoadMapCommand &loadMap) : Command(loadMap)
{
    filename_ = new std::string(*loadMap.filename_);
}

LoadMapCommand::~LoadMapCommand()
{
    delete filename_;
}

LoadMapCommand &LoadMapCommand::operator=(const LoadMapCommand &command)
{
    if (this != &command)
    {
        Command::operator=(command);
        delete filename_;
        filename_ = new std::string(*command.filename_);
    }
    return *this;
}

bool LoadMapCommand::validate()
{
    if (!std::filesystem::exists(*filename_))
    {
        saveEffect("Map file \"" + *filename_ + "\" not found!");
        return false;
    }
    if (gameEngine_->state() == GameState::start || gameEngine_->state() == GameState::mapLoaded)
        return true;
    return Command::validate();
}

void LoadMapCommand::execute()
{
    if (!validate()) return;
    MapLoader::load(*filename_, gameEngine_->map());
    saveEffect("Map \"" + gameEngine_->map().name() + "\" loaded.");
    gameEngine_->transition(GameState::mapLoaded);
}

ValidateMapCommand::ValidateMapCommand(GameEngine &gameEngine) : Command(gameEngine, "ValidateMap") {}

ValidateMapCommand::ValidateMapCommand(const ValidateMapCommand &validateMap) = default;

ValidateMapCommand::~ValidateMapCommand() = default;

ValidateMapCommand &ValidateMapCommand::operator=(const ValidateMapCommand &command)
{
    if (this != &command)
    {
        Command::operator=(command);
    }
    return *this;
}

bool ValidateMapCommand::validate()
{
    if (gameEngine_->state() == GameState::mapLoaded)
        return true;

    return Command::validate();
}

void ValidateMapCommand::execute()
{
    if (!validate() || !gameEngine_->map().validate())
    {
        saveEffect("Map not valid, enter a new map please.");
        return;
    }
    saveEffect("Map \"" + gameEngine_->map().name() + "\" validated.");
    gameEngine_->transition(GameState::mapValidated);
}

AddPlayerCommand::AddPlayerCommand(GameEngine &gameEngine, const std::string &playerName) : Command(gameEngine, "AddPlayer"), playerName_{new std::string(playerName)} {}

AddPlayerCommand::AddPlayerCommand(const AddPlayerCommand &addPlayer) = default;

AddPlayerCommand::~AddPlayerCommand()
{
    delete playerName_;
}

AddPlayerCommand &AddPlayerCommand::operator=(const AddPlayerCommand &command)
{
    if (this != &command)
    {
        Command::operator=(command);
        delete playerName_;
        playerName_ = new std::string(*command.playerName_);
    }
    return *this;
}

bool AddPlayerCommand::validate()
{
    if (gameEngine_->state() == GameState::mapValidated || gameEngine_->state() == GameState::playersAdded)
        return true;
    if (gameEngine_->getPlayers().size() >= 6)
    {
        saveEffect("Maximum of 6 players already reached.");
        return false;
    }
    return Command::validate();
}

void AddPlayerCommand::execute()
{
    if (!validate()) return;
    gameEngine_->getPlayers().push_back(new Player(*gameEngine_, *playerName_));
    saveEffect("Player \"" + *playerName_ + "\" added.");
    gameEngine_->transition(GameState::playersAdded);
}

GameStartCommand::GameStartCommand(GameEngine &gameEngine)
    : Command(gameEngine, "GameStart") {}

GameStartCommand::GameStartCommand(const GameStartCommand &assignTerritories) = default;

GameStartCommand::~GameStartCommand() = default;

GameStartCommand &GameStartCommand::operator=(const GameStartCommand &command)
{
    if (this != &command)
    {
        Command::operator=(command);
    }
    return *this;
}

bool GameStartCommand::validate()
{
    if (gameEngine_->state() == GameState::playersAdded && gameEngine_->getPlayers().size() >= 2)
        return true;
    std::stringstream s;
    s << "Command \"" << *description_ << "\" not valid in state \"" << *gameEngine_ << "\".";
    if (gameEngine_->state() == GameState::playersAdded && gameEngine_->getPlayers().size() < 2)
        s << " You need at least 2 players.";
    saveEffect(s.str());
    return false;
}

void GameStartCommand::execute()
{
    if (!validate()) return;
    auto territories = gameEngine_->map().territories();
    std::random_device r;
    std::default_random_engine e(r());

    std::stringstream s;
    assignTerritories(territories, e, s);

    setPlayerTurnOrder(e, s);

    // This lets each player draw 2 cards
    Deck ourDeck = gameEngine_->getDeck();
    for (auto i : gameEngine_->getPlayers())
    {
        i->draw();
        i->draw();
        s << "\n" << i->getName() << " drew " << i->getHand();
    }
    saveEffect(s.str());
    gameEngine_->transition(GameState::assignReinforcements);
}

void GameStartCommand::setPlayerTurnOrder(std::default_random_engine &e, std::ostream &os) {
    std::shuffle(gameEngine_->getPlayers().begin(), gameEngine_->getPlayers().end(), e);
    os << "Turn Order: ";
    for (auto p : gameEngine_->getPlayers()) {
        os << p->getName() << " ";
    }
}

void GameStartCommand::assignTerritories(std::vector<Territory *> &territories, std::default_random_engine &e, std::ostream &os) {
    os << "Assigned Territories:" << std::endl;
    while (!territories.empty())
    {
        for (auto &it : gameEngine_->getPlayers())
        {
            std::uniform_int_distribution<size_t> u(0, territories.size() - 1);
            auto random = u(e);
            auto territory = territories[random];
            (*it).add(*territory);
            territories.erase(territories.begin() + random);
            os << "\t" <<  territory->name() << " to " << it->getName() << std::endl;
            if (territories.empty())
                break;
        }
    }
}

ReplayCommand::ReplayCommand(GameEngine &gameEngine) : Command(gameEngine, "Play") {}

ReplayCommand::ReplayCommand(const ReplayCommand &play) = default;

ReplayCommand::~ReplayCommand() = default;

ReplayCommand &ReplayCommand::operator=(const ReplayCommand &command)
{
    if (this != &command)
    {
        Command::operator=(command);
    }
    return *this;
}

bool ReplayCommand::validate()
{
    if (gameEngine_->state() == GameState::win)
        return true;

    return Command::validate();
}

void ReplayCommand::execute()
{
    if (!validate()) return;
    saveEffect("Replaying the Conquest game.");
    gameEngine_->transition(GameState::start);
}

QuitCommand::QuitCommand(GameEngine &gameEngine) : Command(gameEngine, "Quit") {}

QuitCommand::QuitCommand(const QuitCommand &quit) = default;

QuitCommand::~QuitCommand() = default;

QuitCommand &QuitCommand::operator=(const QuitCommand &command)
{
    if (this != &command)
    {
        Command::operator=(command);
    }
    return *this;
}

bool QuitCommand::validate()
{
    if (gameEngine_->state() == GameState::win)
        return true;

    return Command::validate();
}

void QuitCommand::execute()
{
    if (!validate()) return;
    saveEffect("Quitting the Conquest game. Thank you for playing!");
    gameEngine_->transition(GameState::gameOver);
}