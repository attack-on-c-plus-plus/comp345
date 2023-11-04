#include "GameEngine.h"
#include <iostream>
#include <cctype>
#include <algorithm>
#include <sstream>

// Daniel Soldera
// Carson Senthilkumar
// Joe El-Khoury
// Henri Stephane Carbon
// Haris Mahmood

std::ostream &operator<<(std::ostream &os, GameState state) {
    switch (state) {
        case GameState::start: os << "start"; break;
        case GameState::mapLoaded: os << "mapLoaded"; break;
        case GameState::mapValidated: os << "mapValidated"; break;
        case GameState::playersAdded: os << "playersAdded"; break;
        case GameState::assignReinforcements: os << "assignReinforcements"; break;
        case GameState::issueOrders: os << "issueOrders"; break;
        case GameState::executeOrders: os << "executeOrders"; break;
        case GameState::win: os << "win"; break;
        default: os.setstate(std::ios_base::failbit);
    }
    return os;
}

/**
 * Main constructor for the GameEngine object
 * @param gameStates
 */
GameEngine::GameEngine(const GameState &state) : state_{new GameState(state)},
    commandProcessor_{new CommandProcessor}, map_{new Map} {
    players_ = new std::vector<Player*>();
}

/**
 * Copy constructor
 * @param gameEngine
 */
GameEngine::GameEngine(const GameEngine &gameEngine) :
    Subject(gameEngine), state_{new GameState(*gameEngine.state_)},
    commandProcessor_{new CommandProcessor}, map_{new Map} {
    players_ = new std::vector<Player*>(*gameEngine.players_);
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
        delete map_;
        delete commandProcessor_;
        players_->clear();
        delete players_;
        state_ = new GameState(*gameEngine.state_);
        players_ = new std::vector<Player*>(*gameEngine.players_);
        map_ = new Map(*gameEngine.map_);
        commandProcessor_ = new CommandProcessor(*gameEngine.commandProcessor_);
    }
    return *this;
}

/**
 * Gets the current game state
 * @return
 */
GameState GameEngine::state() const {
    return *state_;
}
/*
 * Gets the current map
 */
Map GameEngine::getMap() const
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
std::vector<Player *>& GameEngine::getPlayers()
{
    return *players_;
}
/**
 * Determines if the game is over
 * @return true if over; false otherwise
 */
bool GameEngine::gameOver() const
{
    return *state_ == GameState::gameOver;
}

void GameEngine::gameLoop() {
    while (*state_ != GameState::gameOver) {
        std::cout << "Preparing game" << std::endl;
        startup();
        std::cout << "Playing game" << std::endl;
        play();
    }
}

void GameEngine::startup() {
    Command *command;
    while (*state_ != GameState::assignReinforcements) {
        command = &commandProcessor_->getCommand(*this); //readCommand(*this);
        if (command == nullptr) continue;
        transition(command->execute());
    }
}

void GameEngine::play() {
    Command *command;
    while (*state_ != GameState::gameOver) {
        command = &commandProcessor_->getCommand(*this); // readCommand(); //&commandProcessor_->getCommand();
        transition(command->execute());
        delete command;
    }
}

std::ostream &operator<<(std::ostream &os, const GameEngine &gameEngine) {
    switch (*gameEngine.state_) {
        case GameState::start: os << "start"; break;
        case GameState::mapLoaded: os << "map loaded"; break;
        case GameState::mapValidated: os << "map validated"; break;
        case GameState::playersAdded: os << "players added"; break;
        case GameState::assignReinforcements: os << "assign reinforcements"; break;
        case GameState::issueOrders: os << "issue orders"; break;
        case GameState::executeOrders: os << "execute orders"; break;
        case GameState::win: os << "win"; break;
        default: os.setstate(std::ios_base::failbit);
    }
    return os;
}

std::string GameEngine::stringToLog() const {
    std::stringstream s;
    s << "| Game Engine new state: " << *this;
    return s.str();
}

/**
 * Transitions the Game Engine state to the new state
 * @param gameState
 */
void GameEngine::transition(const GameState newState) {
    *state_ = newState;
    Notify(*this);
}

Command::Command(GameEngine &gameEngine, const std::string &description) {
    gameEngine_ = &gameEngine;
    description_ = new std::string(description);
    effect_ = new std::string();
}

Command::~Command() {
    delete description_;
    delete effect_;
}

Command::Command(const Command &command) : Subject(command) {
    gameEngine_ = command.gameEngine_;
    description_ = new std::string (*command.description_);
    effect_ = new std::string(*command.effect_);
}

Command &Command::operator=(const Command &command) {
    if (this != &command)
    {
        delete description_;
        delete effect_;
        gameEngine_ = command.gameEngine_;
        description_ = new std::string (*command.description_);
        effect_ = new std::string(*command.effect_);
    }
    return *this;
}

std::string Command::stringToLog() const {
    std::stringstream s;
    s << "| Command's effect: " << *this;
    return s.str();
}

std::ostream &operator<<(std::ostream &os, const Command &command) {
    os << *command.effect_;
    return os;
}

void Command::saveEffect(const std::string &effect) {
    *effect_ = effect;
    std::cout << effect << std::endl;
    Notify(*this);
}

bool Command::validate() {
    std::stringstream s;
    s << "Command \"" << *description_ << "\" not valid in state \"" << *gameEngine_ << "\".";
    saveEffect(s.str());
    return false;
}

LoadMapCommand::LoadMapCommand(GameEngine &gameEngine, const std::string &filename) :
    Command(gameEngine, "LoadMap"), filename_{new std::string(filename)} {}

LoadMapCommand::LoadMapCommand(const LoadMapCommand &loadMap) : Command(loadMap) {
    filename_ = new std::string(*loadMap.filename_);
}

LoadMapCommand::~LoadMapCommand() {
    delete filename_;
}

LoadMapCommand &LoadMapCommand::operator=(const LoadMapCommand &command) {
    if (this != &command)
    {
        Command::operator=(command);
        delete filename_;
        filename_ = new std::string(*command.filename_);
    }
    return *this;
}

bool LoadMapCommand::validate() {
    if(!std::filesystem::exists(*filename_)) {
        saveEffect("Map file \"" + *filename_ + "\" not found!");
        return false;
    }
    if (gameEngine_->state() == GameState::start || gameEngine_->state() == GameState::mapLoaded)
        return true;
    return Command::validate();
}

GameState LoadMapCommand::execute() {
    if (!validate()) return gameEngine_->state();
    MapLoader::load(*filename_, gameEngine_->map());
    saveEffect("Map \"" + gameEngine_->map().name() + "\" loaded.");
    return GameState::mapLoaded;
}

LoadMapCommand *LoadMapCommand::clone() const {
    return new LoadMapCommand(*this);
}

ValidateMapCommand::ValidateMapCommand(GameEngine &gameEngine) :
    Command(gameEngine, "ValidateMap") {}

ValidateMapCommand::ValidateMapCommand(const ValidateMapCommand &validateMap) = default;

ValidateMapCommand::~ValidateMapCommand() = default;

ValidateMapCommand &ValidateMapCommand::operator=(const ValidateMapCommand &command) {
    if (this != &command)
    {
        Command::operator=(command);
    }
    return *this;
}

bool ValidateMapCommand::validate() {
    if (gameEngine_->state() == GameState::mapLoaded)
        return true;

    return Command::validate();
}

GameState ValidateMapCommand::execute() {
    if (!validate() || !gameEngine_->map().validate())
    {
        saveEffect("Map not valid, enter a new map please.");
        return gameEngine_->state();
    }
    saveEffect("Map \"" + gameEngine_->map().name() + "\" validated.");
    return GameState::mapValidated;
}

ValidateMapCommand *ValidateMapCommand::clone() const {
    return new ValidateMapCommand(*this);
}

AddPlayerCommand::AddPlayerCommand(GameEngine &gameEngine, const std::string &playerName) :
        Command(gameEngine, "AddPlayer"), playerName_{new std::string(playerName)} {}

AddPlayerCommand::AddPlayerCommand(const AddPlayerCommand &addPlayer) = default;

AddPlayerCommand::~AddPlayerCommand() {
    delete playerName_;
}

AddPlayerCommand &AddPlayerCommand::operator=(const AddPlayerCommand &command) {
    if (this != &command)
    {
        Command::operator=(command);
        delete playerName_;
        playerName_ = new std::string(*command.playerName_);
    }
    return *this;
}

bool AddPlayerCommand::validate() {
   if (gameEngine_->state() == GameState::mapValidated || gameEngine_->state() == GameState::playersAdded)
       return true;

    return Command::validate();
}

GameState AddPlayerCommand::execute() {
    if (!validate()) return gameEngine_->state();
    auto *newPlayer = new Player(*playerName_);
    gameEngine_->getPlayers().push_back(newPlayer);
    saveEffect("Player \"" + *playerName_ + "\" added.");
    if (gameEngine_->getPlayers().size() <= 1)
    {
       std::cout << "You still need another player." << std::endl;
    }
    return GameState::playersAdded;
}

AddPlayerCommand *AddPlayerCommand::clone() const {
    return new AddPlayerCommand(*this);
}

AssignTerritoriesCommand::AssignTerritoriesCommand(GameEngine &gameEngine)
    : Command(gameEngine, "AssignTerritories") {}

AssignTerritoriesCommand::AssignTerritoriesCommand(const AssignTerritoriesCommand &assignTerritories) = default;

AssignTerritoriesCommand::~AssignTerritoriesCommand() = default;

AssignTerritoriesCommand &AssignTerritoriesCommand::operator=(const AssignTerritoriesCommand &command) {
    if (this != &command)
    {
        Command::operator=(command);
    }
    return *this;
}

bool AssignTerritoriesCommand::validate() {
    if (gameEngine_->state() == GameState::playersAdded && gameEngine_->getPlayers().size()>=2)
        return true;
    std::stringstream s;
    s << "Invalid command, " << *description_ << " not valid in state " << gameEngine_ << ".";
    if(gameEngine_->state() == GameState::playersAdded && gameEngine_->getPlayers().size() < 2)
        s << "\nYou need at least 2 players.";
    saveEffect(s.str());
    return false;
}

GameState AssignTerritoriesCommand::execute() {
    if (!validate()) return gameEngine_->state();
    std::cout << "Territories assigned." << std::endl;
    return GameState::assignReinforcements;
}

AssignTerritoriesCommand *AssignTerritoriesCommand::clone() const {
    return new AssignTerritoriesCommand(*this);
}

IssueOrdersCommand::IssueOrdersCommand(GameEngine &gameEngine) : Command(gameEngine, "IssueOrders") {}

IssueOrdersCommand::IssueOrdersCommand(const IssueOrdersCommand &issueOrders) = default;

IssueOrdersCommand::~IssueOrdersCommand() = default;

IssueOrdersCommand &IssueOrdersCommand::operator=(const IssueOrdersCommand &command) {
    if (this != &command)
    {
        Command::operator=(command);
    }
    return *this;
}

bool IssueOrdersCommand::validate() {
    if (gameEngine_->state() == GameState::assignReinforcements || gameEngine_->state() == GameState::issueOrders)
        return true;

    return Command::validate();
}

GameState IssueOrdersCommand::execute() {
    if (!validate()) return gameEngine_->state();
    std::cout << "Issue Order." << std::endl;
    return GameState::issueOrders;
}

IssueOrdersCommand *IssueOrdersCommand::clone() const {
    return new IssueOrdersCommand(*this);
}

EndIssueOrdersCommand::EndIssueOrdersCommand(GameEngine &gameEngine) : Command(gameEngine, "EndIssueOrders") {}

EndIssueOrdersCommand::EndIssueOrdersCommand(const EndIssueOrdersCommand &endIssueOrders) = default;

EndIssueOrdersCommand::~EndIssueOrdersCommand() = default;

EndIssueOrdersCommand &EndIssueOrdersCommand::operator=(const EndIssueOrdersCommand &command) {
    if (this != &command)
    {
        Command::operator=(command);
    }
    return *this;
}

bool EndIssueOrdersCommand::validate() {
    if (gameEngine_->state() == GameState::issueOrders)
        return true;
for (auto p : *players_)

    return Command::validate();
}

GameState EndIssueOrdersCommand::execute() {
    if (!validate()) return gameEngine_->state();
    std::cout << "Ended Issue Orders." << std::endl;
    return GameState::executeOrders;
}

EndIssueOrdersCommand *EndIssueOrdersCommand::clone() const {
    return new EndIssueOrdersCommand(*this);
}

ExecuteOrdersCommand::ExecuteOrdersCommand(GameEngine &gameEngine) : Command(gameEngine, "ExecuteOrders") {}

ExecuteOrdersCommand::ExecuteOrdersCommand(const ExecuteOrdersCommand &executeOrders) = default;

ExecuteOrdersCommand::~ExecuteOrdersCommand() = default;

ExecuteOrdersCommand &ExecuteOrdersCommand::operator=(const ExecuteOrdersCommand &command) {
    if (this != &command)
    {
        Command::operator=(command);
    }
    return *this;
}

bool ExecuteOrdersCommand::validate() {
    if (gameEngine_->state() == GameState::executeOrders)
        return true;

    return Command::validate();
}

GameState ExecuteOrdersCommand::execute() {
    if (!validate()) return gameEngine_->state();
    saveEffect("Execute Order.");
    return GameState::executeOrders;
}

ExecuteOrdersCommand *ExecuteOrdersCommand::clone() const {
    return new ExecuteOrdersCommand(*this);
}

EndExecuteOrdersCommand::EndExecuteOrdersCommand(GameEngine &gameEngine) : Command(gameEngine,"EndExecuteOrders") {}

EndExecuteOrdersCommand::EndExecuteOrdersCommand(const EndExecuteOrdersCommand &endExecuteOrders) = default;

EndExecuteOrdersCommand::~EndExecuteOrdersCommand() = default;

EndExecuteOrdersCommand &EndExecuteOrdersCommand::operator=(const EndExecuteOrdersCommand &command) {
    if (this != &command)
    {
        Command::operator=(command);
    }
    return *this;
}

bool EndExecuteOrdersCommand::validate() {
    if (gameEngine_->state() == GameState::executeOrders)
        return true;

    return Command::validate();
}

GameState EndExecuteOrdersCommand::execute() {
    if (!validate()) return gameEngine_->state();
    std::cout << "Ended Execute Orders." << std::endl;
    return GameState::assignReinforcements;
}

EndExecuteOrdersCommand *EndExecuteOrdersCommand::clone() const {
    return new EndExecuteOrdersCommand(*this);
}

WinCommand::WinCommand(GameEngine &gameEngine) : Command(gameEngine, "Win") {}

WinCommand::WinCommand(const WinCommand &win) = default;

WinCommand::~WinCommand() = default;

WinCommand &WinCommand::operator=(const WinCommand &command) {
    if (this != &command)
    {
        Command::operator=(command);
    }
    return *this;
}

bool WinCommand::validate() {
    if (gameEngine_->state() == GameState::executeOrders)
        return true;

    return Command::validate();
}

GameState WinCommand::execute() {
    if (!validate()) return gameEngine_->state();
    std::cout << "WinCommand." << std::endl;
    return GameState::win;
}

WinCommand *WinCommand::clone() const {
    return new WinCommand(*this);
}

PlayCommand::PlayCommand(GameEngine &gameEngine) : Command(gameEngine, "Play") {}

PlayCommand::PlayCommand(const PlayCommand &play) = default;

PlayCommand::~PlayCommand() = default;

PlayCommand &PlayCommand::operator=(const PlayCommand &command) {
    if (this != &command)
    {
        Command::operator=(command);
    }
    return *this;
}

bool PlayCommand::validate() {
    if (gameEngine_->state() == GameState::win)
        return true;

    return Command::validate();
}

GameState PlayCommand::execute() {
    if (!validate()) return gameEngine_->state();
    std::cout << "Replaying." << std::endl;
    return GameState::start;
}

PlayCommand *PlayCommand::clone() const {
    return new PlayCommand(*this);
}

QuitCommand::QuitCommand(GameEngine &gameEngine) : Command(gameEngine, "Quit") {}

QuitCommand::QuitCommand(const QuitCommand &quit) = default;

QuitCommand::~QuitCommand() = default;

QuitCommand &QuitCommand::operator=(const QuitCommand &command) {
    if (this != &command)
    {
        Command::operator=(command);
    }
    return *this;
}

bool QuitCommand::validate() {
    if (gameEngine_->state() == GameState::win)
        return true;

    return Command::validate();
}

GameState QuitCommand::execute() {
    if (!validate()) return gameEngine_->state();
    std::cout << "Quitting" << std::endl;
    return GameState::gameOver;
}

QuitCommand *QuitCommand::clone() const {
    return new QuitCommand(*this);
}
