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

#include "GameEngine.h"

#include <algorithm>
#include <cassert>
#include <climits>
#include <filesystem>
#include <iostream>
#include <random>
#include <sstream>

#include "Cards.h"
#include "CommandProcessing.h"
#include "Map.h"
#include "Orders.h"
#include "Player.h"

/**
 * Main constructor for the GameEngine object
 * @param commandProcessor
 * @param random
 */
GameEngine::GameEngine(CommandProcessor &commandProcessor, const IRandom &random) : state_{new GameState(GameState::start)},
    previousState_{new GameState(GameState::start)}, map_{new Map} {
    commandProcessor_ = &commandProcessor;
    players_ = new Players(*this);
    deck_ = new Deck(random, 24);
    random_ = &random;
}

/**
 * Destructor
 */
GameEngine::~GameEngine() {
    delete state_;
    delete previousState_;
    delete map_;
    delete players_;
    delete deck_;
}

/**GameEngine *gameEngine_;
 * Gets the current game state
 * @return
 */
GameState GameEngine::state() const {
    return *state_;
}

GameState GameEngine::previousState() const {
    return *previousState_;
}

/*
 * Gets the current map
 */
Map& GameEngine::map() const {
    return *map_;
}

/*
 * Sets the map to the gameEngine
 */
void GameEngine::map(Map&map) {
    map_ = &map;
}

/**
 * Gets the vector containing the players
 */
Players& GameEngine::players() const {
    return *players_;
}

/**
 * Gets the Deck
 */
Deck& GameEngine::deck() const {
    return *deck_;
}

/**
 * Determines if the game is over
 * @return true if over; false otherwise
 */
bool GameEngine::gameOver() const {
    return *state_ == GameState::gameOver;
}

void GameEngine::gameLoop() {
    while (*state_ != GameState::gameOver) {
        std::cout << "Preparing game" << std::endl;
        startup();
        std::cout << "Playing game" << std::endl;
        mainGameLoop();
    }
}

void GameEngine::startup() {
    resetGameElements();
    while (*state_ != GameState::assignReinforcements) {
        Command* command = &commandProcessor_->getCommand(*this);
        if (command == nullptr) continue;
        command->execute();
    }
}

void GameEngine::resetGameElements() {
    delete map_;
    delete players_;
    map_ = new Map();
    players_ = new Players(*this);
}

void GameEngine::mainGameLoop() {
    while (*state_ != GameState::win) {
        reinforcementPhase();
        issuingOrderPhase();
        executeOrdersPhase();
    }
    gameOverPhase();
}

void GameEngine::gameOverPhase() {
    while (*state_ == GameState::win) {
        Command* command = &commandProcessor_->getCommand(*this);
        command->execute();
    }
}

std::string GameEngine::stringToLog() const {
    std::stringstream s;
    s << "| Game Engine new state: " << *this;
    if (*state_ == GameState::win)
        s << " player " << players_->player(0) << " wins!";
    return s.str();
}

/**
 * Transitions the Game Engine state to the new state
 * @param newState game state to transition to
 */
void GameEngine::transition(const GameState newState) {
    *previousState_ = *state_;
    *state_ = newState;
    Notify(*this);
}

const IRandom& GameEngine::random() const {
    return *random_;
}

/**
 * Executes players orders
 */
void GameEngine::executeOrdersPhase() const {
    if (*state_ != GameState::executeOrders)
        return; // Game engine is in the wrong state

    const std::string seperator(70, '=');
    std::cout << seperator << std::endl;
    std::cout << "Execute Order Phase" << std::endl;
    std::cout << seperator << std::endl;

    players_->executeOrders();
}

/**
 * Executes the reinforcement phase
 */
void GameEngine::reinforcementPhase() {
    if (*state_ != GameState::assignReinforcements)
        return;
    const std::string seperator(70, '=');
    std::cout << seperator << std::endl;
    std::cout << "Reinforcement Phase" << std::endl;
    std::cout << seperator << std::endl;
    map_->print();
    players_->reinforcement();

    transition(GameState::issueOrders);
}

/**
 * Executes the issue orders phase
 */
void GameEngine::issuingOrderPhase() {
    if (*state_ != GameState::issueOrders)
        return;

    const std::string seperator(70, '=');
    std::cout << seperator << std::endl;
    std::cout << "Issuing Order Phase" << std::endl;
    std::cout << seperator << std::endl;
    players_->issueOrders();
    transition(GameState::executeOrders);
}

std::ostream& operator<<(std::ostream&os, const GameEngine&gameEngine) {
    switch (*gameEngine.state_) {
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

Command::Command(GameEngine&gameEngine, const std::string&description) {
    gameEngine_ = &gameEngine;
    description_ = new std::string(description);
    effect_ = new std::string();
}

Command::~Command() {
    delete description_;
    delete effect_;
}

Command::Command(const Command&command) : Subject(command) {
    gameEngine_ = command.gameEngine_;
    description_ = new std::string(*command.description_);
    effect_ = new std::string(*command.effect_);
}

Command& Command::operator=(const Command&command) {
    if (this != &command) {
        delete description_;
        delete effect_;
        gameEngine_ = command.gameEngine_;
        description_ = new std::string(*command.description_);
        effect_ = new std::string(*command.effect_);
    }
    return *this;
}

std::string& Command::description() const {
    return *description_;
}

std::string Command::stringToLog() const {
    std::stringstream s;
    s << "| Command's Effect: " << *effect_;
    return s.str();
}

std::ostream& operator<<(std::ostream&os, const Command&command) {
    os << *command.effect_;
    return os;
}

void Command::saveEffect(const std::string&effect) {
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

LoadMapCommand::LoadMapCommand(GameEngine&gameEngine, const std::string&filename) : Command(gameEngine, "LoadMap"),
    filename_{new std::string(filename)} {
}

LoadMapCommand::LoadMapCommand(const LoadMapCommand&loadMap) : Command(loadMap) {
    filename_ = new std::string(*loadMap.filename_);
}

LoadMapCommand::~LoadMapCommand() {
    delete filename_;
}

LoadMapCommand& LoadMapCommand::operator=(const LoadMapCommand&command) {
    if (this != &command) {
        Command::operator=(command);
        delete filename_;
        filename_ = new std::string(*command.filename_);
    }
    return *this;
}

bool LoadMapCommand::validate() {
    if (!std::filesystem::exists(*filename_)) {
        saveEffect("Map file \"" + *filename_ + "\" not found!");
        return false;
    }
    if (gameEngine_->state() == GameState::start || gameEngine_->state() == GameState::mapLoaded)
        return true;
    return Command::validate();
}

void LoadMapCommand::execute() {
    if (!validate()) return;
    MapLoader::load(*filename_, gameEngine_->map());
    saveEffect("Map \"" + gameEngine_->map().name() + "\" loaded.");
    gameEngine_->transition(GameState::mapLoaded);
}

ValidateMapCommand::ValidateMapCommand(GameEngine&gameEngine) : Command(gameEngine, "ValidateMap") {
}

ValidateMapCommand::ValidateMapCommand(const ValidateMapCommand&validateMap) = default;

ValidateMapCommand::~ValidateMapCommand() = default;

ValidateMapCommand& ValidateMapCommand::operator=(const ValidateMapCommand&command) {
    if (this != &command) {
        Command::operator=(command);
    }
    return *this;
}

bool ValidateMapCommand::validate() {
    if (gameEngine_->state() == GameState::mapLoaded)
        return true;

    return Command::validate();
}

void ValidateMapCommand::execute() {
    if (!validate() || !gameEngine_->map().validate()) {
        saveEffect("Map not valid, enter a new map please.");
        return;
    }
    saveEffect("Map \"" + gameEngine_->map().name() + "\" validated.");
    gameEngine_->transition(GameState::mapValidated);
}

AddPlayerCommand::AddPlayerCommand(GameEngine&gameEngine, const std::string&playerName, const Strategy strategy) : Command(gameEngine,
    "AddPlayer"), playerName_{new std::string(playerName)}, strategy_{new Strategy(strategy)} {
}

AddPlayerCommand::AddPlayerCommand(const AddPlayerCommand&addPlayer) = default;

AddPlayerCommand::~AddPlayerCommand() {
    delete playerName_;
}

AddPlayerCommand& AddPlayerCommand::operator=(const AddPlayerCommand&command) {
    if (this != &command) {
        Command::operator=(command);
        delete playerName_;
        playerName_ = new std::string(*command.playerName_);
    }
    return *this;
}

bool AddPlayerCommand::validate() {
    if (gameEngine_->state() == GameState::mapValidated || gameEngine_->state() == GameState::playersAdded)
        return true;
    if (gameEngine_->players().hasMaximum()) {
        saveEffect("Maximum of 6 players already reached.");
        return false;
    }
    return Command::validate();
}

void AddPlayerCommand::execute() {
    if (!validate()) return;
    if (Player p(*gameEngine_, *playerName_, *strategy_); !gameEngine_->players().add(p)) {
        saveEffect("Player \"" + *playerName_ + "\" already in game. Not added.");
        return;
    }
    saveEffect("Player \"" + *playerName_ + "\" added.");
    gameEngine_->transition(GameState::playersAdded);
}

GameStartCommand::GameStartCommand(GameEngine&gameEngine)
    : Command(gameEngine, "GameStart") {
}

GameStartCommand::GameStartCommand(const GameStartCommand&assignTerritories) = default;

GameStartCommand::~GameStartCommand() = default;

GameStartCommand& GameStartCommand::operator=(const GameStartCommand&command) {
    if (this != &command) {
        Command::operator=(command);
    }
    return *this;
}

bool GameStartCommand::validate() {
    if (gameEngine_->state() == GameState::playersAdded && gameEngine_->players().hasMinimum())
        return true;
    std::stringstream s;
    s << "Command \"" << *description_ << "\" not valid in state \"" << *gameEngine_ << "\".";
    if (gameEngine_->state() == GameState::playersAdded && !gameEngine_->players().hasMinimum())
        s << " You need at least 2 players.";
    saveEffect(s.str());
    return false;
}

void GameStartCommand::execute() {
    if (!validate()) return;
    std::stringstream os;
    gameEngine_->players().init(os);
    saveEffect(os.str());
    gameEngine_->transition(GameState::assignReinforcements);
}

ReplayCommand::ReplayCommand(GameEngine&gameEngine) : Command(gameEngine, "Play") {
}

ReplayCommand::ReplayCommand(const ReplayCommand&play) = default;

ReplayCommand::~ReplayCommand() = default;

ReplayCommand& ReplayCommand::operator=(const ReplayCommand&command) {
    if (this != &command) {
        Command::operator=(command);
    }
    return *this;
}

bool ReplayCommand::validate() {
    if (gameEngine_->state() == GameState::win)
        return true;

    return Command::validate();
}

void ReplayCommand::execute() {
    if (!validate()) return;
    saveEffect("Replaying the Conquest game.");
    gameEngine_->transition(GameState::start);
}

QuitCommand::QuitCommand(GameEngine&gameEngine) : Command(gameEngine, "Quit") {
}

QuitCommand::QuitCommand(const QuitCommand&quit) = default;

QuitCommand::~QuitCommand() = default;

QuitCommand& QuitCommand::operator=(const QuitCommand&command) {
    if (this != &command) {
        Command::operator=(command);
    }
    return *this;
}

bool QuitCommand::validate() {
    if (gameEngine_->state() == GameState::win)
        return true;

    return Command::validate();
}

void QuitCommand::execute() {
    if (!validate()) return;
    saveEffect("Quitting the Conquest game. Thank you for playing!");
    gameEngine_->transition(GameState::gameOver);
}

/**
 * \brief
 */
Random::Random() {
    eng = new std::default_random_engine[1];
    std::random_device r{};
    std::default_random_engine e(r());
    eng[0] = e;
    u = new std::uniform_int_distribution<unsigned>(0, UINT_MAX);
}

Random::~Random() {
    delete [] eng;
    delete u;
}

void Random::setPlayOrder(std::vector<Player*>& players) const {
    std::ranges::shuffle(players, eng[0]);
}

unsigned Random::generate(const unsigned from, const unsigned to) const {
    assert(from <= to);
    return (*u)(eng[0]) % (to - from + 1) + from;
}

FakeRandom::FakeRandom() {
    eng = new std::mt19937[1];
    std::mt19937 e{123};
    eng[0] = e;
    u = new std::uniform_int_distribution<unsigned>(0, UINT_MAX);
}

FakeRandom::~FakeRandom() {
    delete [] eng;
    delete u;
}

void FakeRandom::setPlayOrder(std::vector<Player*>& players) const {
    std::ranges::shuffle(players, eng[0]);
}

unsigned FakeRandom::generate(const unsigned from, const unsigned to) const {
    assert(from <= to);
    return (*u)(eng[0]) % (to - from + 1) + from;
}
