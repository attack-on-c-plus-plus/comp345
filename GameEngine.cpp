#include "GameEngine.h"
#include <iostream>
#include <cctype>
#include <algorithm>

// Daniel Soldera
// Carson Senthilkumar
// Joe El-Khoury
// Henri Stephane Carbon
// Haris Mahmood

/**
 * Main constructor for the GameEngine object
 * @param gameStates
 */
GameEngine::GameEngine(const GameState &state) :
        state_{new GameState(state)} {}

/**
 * Copy constructor
 * @param gameEngine
 */
GameEngine::GameEngine(const GameEngine &gameEngine) :
        state_{new GameState(*gameEngine.state_)} {}

/**
 * Destructor
 */
GameEngine::~GameEngine()
{
    delete state_;
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
        state_ = new GameState(*gameEngine.state_);
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

/**
 * Determines if the game is over
 * @return true if over; false otherwise
 */
bool GameEngine::gameOver() const
{
    return *state_ == GameState::win;
}

void GameEngine::gameLoop() {
    while (*state_ != GameState::gameover) {
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
        *state_ = command->execute();
        delete command;
    }
}

void GameEngine::play() {
    Command *command;
    while (*state_ != GameState::gameover) {
        command = readCommand();
        if (command == nullptr) continue;
        *state_ = command->execute();
        delete command;
    }
}

std::ostream &operator<<(std::ostream &os, GameEngine &gameEngine) {
    os << "GameEngine state: ";
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
    else if (s == "assigncountries") {return new AssignTerritoriesCommand(*this);}
    else if (s == "issueorder") {return new IssueOrdersCommand(*this);}
    else if (s == "endissueorders") {return new EndIssueOrdersCommand(*this);}
    else if (s == "execorder") {return new ExecuteOrdersCommand(*this);}
    else if (s == "endexecorders") {return new EndExecuteOrdersCommand(*this);}
    else if (s == "win") {return new WinCommand(*this);}
    else if (s == "play") {return new PlayCommand(*this);}
    else if (s == "end") {return new QuitCommand(*this);}
    else { std::cout << "Invalid command. " << s << std::endl; return nullptr; }
}

Command::Command(GameEngine &gameEngine) {
        gameEngine_ = &gameEngine;
}

Command::~Command() {
    gameEngine_ = nullptr;
}

Command &Command::operator=(const Command &command) {
    if (this != &command)
    {
        gameEngine_ = nullptr;
        gameEngine_ = command.gameEngine_;
    }
    return *this;
}

Command::Command(const Command &command) {
    gameEngine_ = command.gameEngine_;
}

LoadMapCommand::LoadMapCommand(GameEngine &gameEngine) :
    Command(gameEngine) {}

LoadMapCommand::~LoadMapCommand() {

}

bool LoadMapCommand::valid() {
    if (gameEngine_->state() == GameState::start || gameEngine_->state() == GameState::mapLoaded)
        return true;

    std::cout << "Invalid command, please try again, your current state is start." << std::endl;
    return false;
}

GameState LoadMapCommand::execute() {
    if (!valid()) return gameEngine_->state();
    std::cout << "Map loaded." << std::endl;
    return GameState::mapLoaded;
}

LoadMapCommand *LoadMapCommand::clone() const {
    return new LoadMapCommand(*this);
}

LoadMapCommand &LoadMapCommand::operator=(const LoadMapCommand &command) {
    return *this;
}

LoadMapCommand::LoadMapCommand(const LoadMapCommand &loadMap) :
    Command(loadMap) {

}

ValidateMapCommand::ValidateMapCommand(GameEngine &gameEngine) :
    Command(gameEngine) {}

ValidateMapCommand::~ValidateMapCommand() {

}

bool ValidateMapCommand::valid() {
    if (gameEngine_->state() == GameState::mapLoaded)
        return true;

    std::cout << "Invalid command, please try again." << *gameEngine_ << std::endl;
    return false;
}

GameState ValidateMapCommand::execute() {
    if (!valid()) return gameEngine_->state();
    std::cout << "Map validated." << std::endl;
    return GameState::mapValidated;
}

ValidateMapCommand *ValidateMapCommand::clone() const {
    return new ValidateMapCommand(*this);
}

ValidateMapCommand &ValidateMapCommand::operator=(const ValidateMapCommand &command) {
    return *this;
}

ValidateMapCommand::ValidateMapCommand(const ValidateMapCommand &validateMap) :
    Command(validateMap) {

}

AddPlayerCommand::AddPlayerCommand(GameEngine &gameEngine) : Command(gameEngine) {

}

AddPlayerCommand::~AddPlayerCommand() {

}

bool AddPlayerCommand::valid() {
   if (gameEngine_->state() == GameState::mapValidated || gameEngine_->state() == GameState::playersAdded)
       return true;

    std::cout << "Invalid command, please try again." << *gameEngine_ << std::endl;
    return false;
}

GameState AddPlayerCommand::execute() {
    if (!valid()) return gameEngine_->state();
    std::cout << "Player added." << std::endl;
    return GameState::playersAdded;
}

AddPlayerCommand *AddPlayerCommand::clone() const {
    return new AddPlayerCommand(*this);
}

AddPlayerCommand &AddPlayerCommand::operator=(const AddPlayerCommand &command) {
    return *this;
}

AddPlayerCommand::AddPlayerCommand(const AddPlayerCommand &addPlayer) :
    Command(addPlayer) {

}

AssignTerritoriesCommand::AssignTerritoriesCommand(GameEngine &gameEngine) :
    Command(gameEngine) {

}

AssignTerritoriesCommand::~AssignTerritoriesCommand() {

}

bool AssignTerritoriesCommand::valid() {
    if (gameEngine_->state() == GameState::playersAdded)
        return true;

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

AssignTerritoriesCommand &AssignTerritoriesCommand::operator=(const AssignTerritoriesCommand &command) {
    return *this;
}

AssignTerritoriesCommand::AssignTerritoriesCommand(const AssignTerritoriesCommand &assignTerritories) :
    Command(assignTerritories) {

}

IssueOrdersCommand::IssueOrdersCommand(GameEngine &gameEngine) : Command(gameEngine) {

}

IssueOrdersCommand::~IssueOrdersCommand() {

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

IssueOrdersCommand &IssueOrdersCommand::operator=(const LoadMapCommand &command) {
    return *this;
}

IssueOrdersCommand::IssueOrdersCommand(const IssueOrdersCommand &issueOrders) :
        Command(issueOrders){

}

EndIssueOrdersCommand::EndIssueOrdersCommand(GameEngine &gameEngine) : Command(gameEngine) {

}

EndIssueOrdersCommand::~EndIssueOrdersCommand() {

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

EndIssueOrdersCommand &EndIssueOrdersCommand::operator=(const EndIssueOrdersCommand &command) {
    return *this;
}

EndIssueOrdersCommand::EndIssueOrdersCommand(const EndIssueOrdersCommand &endIssueOrders) :
    Command(endIssueOrders) {

}

ExecuteOrdersCommand::ExecuteOrdersCommand(GameEngine &gameEngine) : Command(gameEngine) {

}

ExecuteOrdersCommand::~ExecuteOrdersCommand() {

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

ExecuteOrdersCommand &ExecuteOrdersCommand::operator=(const ExecuteOrdersCommand &command) {
    return *this;
}

ExecuteOrdersCommand::ExecuteOrdersCommand(const ExecuteOrdersCommand &executeOrders) :
    Command(executeOrders) {

}

EndExecuteOrdersCommand::EndExecuteOrdersCommand(GameEngine &gameEngine) : Command(gameEngine) {

}

EndExecuteOrdersCommand::~EndExecuteOrdersCommand() {

}

bool EndExecuteOrdersCommand::valid() {
    if (gameEngine_->state() == GameState::executeOrders)
        return true;

    std::cout << "Invalid command, please try again." << *gameEngine_ << std::endl;
    return false;
}

GameState EndExecuteOrdersCommand::execute() {
    if (!valid()) return gameEngine_->state();
    std::cout << "Ended Issue Orders." << std::endl;
    return GameState::assignReinforcements;
}

EndExecuteOrdersCommand *EndExecuteOrdersCommand::clone() const {
    return new EndExecuteOrdersCommand(*this);
}

EndExecuteOrdersCommand &EndExecuteOrdersCommand::operator=(const EndExecuteOrdersCommand &command) {
    return *this;
}

EndExecuteOrdersCommand::EndExecuteOrdersCommand(const EndExecuteOrdersCommand &endExecuteOrders) :
    Command(endExecuteOrders) {

}

WinCommand::WinCommand(GameEngine &gameEngine) : Command(gameEngine) {

}

WinCommand::~WinCommand() {

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

WinCommand &WinCommand::operator=(const WinCommand &command) {
    return *this;
}

WinCommand::WinCommand(const WinCommand &win) :
    Command(win) {

}

PlayCommand::PlayCommand(GameEngine &gameEngine) : Command(gameEngine) {

}

PlayCommand::~PlayCommand() {

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

PlayCommand &PlayCommand::operator=(const PlayCommand &command) {
    return *this;
}

PlayCommand::PlayCommand(const PlayCommand &play) :
    Command(play) {

}

QuitCommand::QuitCommand(GameEngine &gameEngine) : Command(gameEngine) {

}

QuitCommand::~QuitCommand() {

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
    return GameState::gameover;
}

QuitCommand *QuitCommand::clone() const {
    return new QuitCommand(*this);
}

QuitCommand &QuitCommand::operator=(const QuitCommand &command) {
    return *this;
}

QuitCommand::QuitCommand(const QuitCommand &quit) :
    Command(quit) {

}
