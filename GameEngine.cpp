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

/**
 * Main constructor for the GameEngine object
 * @param gameStates
 */
GameEngine::GameEngine(const GameState &state) : state_{new GameState(state)}, map_{new Map} {
    players_ = new std::vector<Player*>();
}

/**
 * Copy constructor
 * @param gameEngine
 */
GameEngine::GameEngine(const GameEngine &gameEngine) :
    Subject(gameEngine),
    state_{new GameState(*gameEngine.state_)}, map_{new Map(*gameEngine.map_)} {
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
        players_->clear();
        delete players_;
        state_ = new GameState(*gameEngine.state_);
        map_ = new Map(*gameEngine.map_);
        players_ = new std::vector<Player*>(*gameEngine.players_);
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
 * Gets the vector containing the players_
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
        command = readCommand();
        if (command == nullptr) continue;
        transition(command->execute());
        delete command;
    }
}

void GameEngine::play() {
    Command *command;
    while (*state_ != GameState::gameOver) {
        command = readCommand();
        if (command == nullptr) continue;
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

Command *GameEngine::readCommand() {
    std::string s;
    std::cout << "Enter your command." << std::endl;
    std::cin >> s;
    std::transform(s.begin(), s.end(), s.begin(), ::tolower);

    if (s == "loadmap") { return new LoadMapCommand(*this); }
    else if (s == "validatemap") { return new ValidateMapCommand(*this); }
    else if (s == "addplayer") { return new AddPlayerCommand(*this); }
    else if (s == "assignterritories") {return new AssignTerritoriesCommand(*this);}
    else if (s == "issueorder") {return new IssueOrdersCommand(*this);}
    else if (s == "endissueorders") {return new EndIssueOrdersCommand(*this);}
    else if (s == "execorder") {return new ExecuteOrdersCommand(*this);}
    else if (s == "endexecorders") {return new EndExecuteOrdersCommand(*this);}
    else if (s == "win") {return new WinCommand(*this);}
    else if (s == "play") {return new PlayCommand(*this);}
    else if (s == "end") {return new QuitCommand(*this);}
    else { std::cout << "Invalid command. " << s << std::endl; return nullptr; }
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
    Notify(*this);
}

LoadMapCommand::LoadMapCommand(GameEngine &gameEngine) :
    Command(gameEngine, "LoadMap") {}

LoadMapCommand::LoadMapCommand(const LoadMapCommand &loadMap) = default;

LoadMapCommand::~LoadMapCommand() = default;

LoadMapCommand &LoadMapCommand::operator=(const LoadMapCommand &command) {
    if (this != &command)
    {
        Command::operator=(command);
    }
    return *this;
}

bool LoadMapCommand::valid() {
    if (gameEngine_->state() == GameState::start || gameEngine_->state() == GameState::mapLoaded)
        return true;

    std::cout << "Invalid command, please try again, your current state is start." << std::endl;
    return false;
}

GameState LoadMapCommand::execute() {
    if (!valid()) return gameEngine_->state();
    while (true)
    {
        std::cout << "Enter the file path for the map you want" << std::endl;
        std::string file;
        std::cin.ignore();
        std::getline(std::cin, file);
        if (MapLoader::load(file, gameEngine_->map()))
        {
            std::cout << "Map loaded." << std::endl;
            return GameState::mapLoaded;
        }
    }
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

bool ValidateMapCommand::valid() {
    if (gameEngine_->state() == GameState::mapLoaded)
        return true;

    std::cout << "Invalid command, please try again." << *gameEngine_ << std::endl;
    return false;
}

GameState ValidateMapCommand::execute() {
    if (!valid()) return gameEngine_->state();
    if (gameEngine_->map().validate())
    {
        std::cout << "Map validated." << std::endl;
        return GameState::mapValidated;
    }
    else
    {
        std::cout << "Map not valid, enter a new map please." << std::endl;
        return gameEngine_->state();
    }
}

ValidateMapCommand *ValidateMapCommand::clone() const {
    return new ValidateMapCommand(*this);
}

AddPlayerCommand::AddPlayerCommand(GameEngine &gameEngine) : Command(gameEngine, "AddPlayer") {}

AddPlayerCommand::AddPlayerCommand(const AddPlayerCommand &addPlayer) = default;

AddPlayerCommand::~AddPlayerCommand() = default;

AddPlayerCommand &AddPlayerCommand::operator=(const AddPlayerCommand &command) {
    if (this != &command)
    {
        Command::operator=(command);
    }
    return *this;
}

bool AddPlayerCommand::valid() {
   if (gameEngine_->state() == GameState::mapValidated || gameEngine_->state() == GameState::playersAdded)
       return true;

    std::cout << "Invalid command, please try again." << *gameEngine_ << std::endl;
    return false;
}

GameState AddPlayerCommand::execute() {
    if (!valid()) return gameEngine_->state();
    if (gameEngine_->getPlayers().size() >= 6)
    {
       std::cout << "Already reached maximum player count of 6." << std::endl;
       return gameEngine_->state();
    }
    std::cout << "Enter Name of Player" << std::endl;
    std::string name;
    std::cin >> name;
    Player *newPlayer = new Player(name);
    gameEngine_->getPlayers().push_back(newPlayer);
    std::cout << "Player added." << std::endl;
    if (gameEngine_->getPlayers().size() <= 1)
    {
       std::cout << "You still need another player." << std::endl;
    }
    return GameState::playersAdded;
}

AddPlayerCommand *AddPlayerCommand::clone() const {
    return new AddPlayerCommand(*this);
}

AssignTerritoriesCommand::AssignTerritoriesCommand(GameEngine &gameEngine) :
    Command(gameEngine, "AssignTerritories") {}

AssignTerritoriesCommand::AssignTerritoriesCommand(const AssignTerritoriesCommand &assignTerritories) = default;

AssignTerritoriesCommand::~AssignTerritoriesCommand() = default;

AssignTerritoriesCommand &AssignTerritoriesCommand::operator=(const AssignTerritoriesCommand &command) {
    if (this != &command)
    {
        Command::operator=(command);
    }
    return *this;
}

bool AssignTerritoriesCommand::valid() {
    if (gameEngine_->state() == GameState::playersAdded && gameEngine_->getPlayers().size()>=2)
        return true;
    if(gameEngine_->state() == GameState::playersAdded && gameEngine_->getPlayers().size()<2){
        std::cout <<"You still need at least 2 players to assign territories" << std::endl;
    }
    std::cout << "Invalid command, please try again." << *gameEngine_ << std::endl;
    return false;
}

GameState AssignTerritoriesCommand::execute() {
    if (!valid()) return gameEngine_->state();
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

bool IssueOrdersCommand::valid() {
    if (gameEngine_->state() == GameState::assignReinforcements || gameEngine_->state() == GameState::issueOrders)
        return true;

    std::cout << "Invalid command, please try again." << *gameEngine_ << std::endl;
    return false;
}

GameState IssueOrdersCommand::execute() {
    if (!valid()) return gameEngine_->state();
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

bool EndIssueOrdersCommand::valid() {
    if (gameEngine_->state() == GameState::issueOrders)
        return true;

    std::cout << "Invalid command, please try again." << *gameEngine_ << std::endl;
    return false;
}

GameState EndIssueOrdersCommand::execute() {
    if (!valid()) return gameEngine_->state();
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

bool ExecuteOrdersCommand::valid() {
    if (gameEngine_->state() == GameState::executeOrders)
        return true;

    std::cout << "Invalid command, please try again." << *gameEngine_ << std::endl;
    return false;
}

GameState ExecuteOrdersCommand::execute() {
    if (!valid()) return gameEngine_->state();
    std::cout << "Execute Order." << std::endl;
    return GameState::executeOrders;
}

ExecuteOrdersCommand *ExecuteOrdersCommand::clone() const {
    return new ExecuteOrdersCommand(*this);
}

EndExecuteOrdersCommand::EndExecuteOrdersCommand(GameEngine &gameEngine) :
    Command(gameEngine, "EndExecuteOrders") {}

EndExecuteOrdersCommand::EndExecuteOrdersCommand(const EndExecuteOrdersCommand &endExecuteOrders) = default;

EndExecuteOrdersCommand::~EndExecuteOrdersCommand() = default;

EndExecuteOrdersCommand &EndExecuteOrdersCommand::operator=(const EndExecuteOrdersCommand &command) {
    if (this != &command)
    {
        Command::operator=(command);
    }
    return *this;
}

bool EndExecuteOrdersCommand::valid() {
    if (gameEngine_->state() == GameState::executeOrders)
        return true;

    std::cout << "Invalid command, please try again." << *gameEngine_ << std::endl;
    return false;
}

GameState EndExecuteOrdersCommand::execute() {
    if (!valid()) return gameEngine_->state();
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

bool WinCommand::valid() {
    if (gameEngine_->state() == GameState::executeOrders)
        return true;

    std::cout << "Invalid command, please try again." << *gameEngine_ << std::endl;
    return false;
}

GameState WinCommand::execute() {
    if (!valid()) return gameEngine_->state();
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

bool PlayCommand::valid() {
    if (gameEngine_->state() == GameState::win)
        return true;

    std::cout << "Invalid command, please try again." << *gameEngine_ << std::endl;
    return false;
}

GameState PlayCommand::execute() {
    if (!valid()) return gameEngine_->state();
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

bool QuitCommand::valid() {
    if (gameEngine_->state() == GameState::win)
        return true;

    std::cout << "Invalid command, please try again." << *gameEngine_ << std::endl;
    return false;
}

GameState QuitCommand::execute() {
    if (!valid()) return gameEngine_->state();
    std::cout << "Quitting" << std::endl;
    return GameState::gameOver;
}

QuitCommand *QuitCommand::clone() const {
    return new QuitCommand(*this);
}
