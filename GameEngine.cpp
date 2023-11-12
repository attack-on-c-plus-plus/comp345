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
 */
GameEngine::GameEngine(CommandProcessor&commandProcessor) : state_{new GameState(GameState::start)},
    previousState_{new GameState(GameState::start)}, map_{new Map} {
    commandProcessor_ = &commandProcessor;
    players_ = new std::vector<Player *>();
    deck_ = new Deck(24);
}

/**
 * Copy constructor
 * @param gameEngine
 */
GameEngine::GameEngine(const GameEngine &gameEngine) : Subject(gameEngine), state_{new GameState(*gameEngine.state_)},
    previousState_{new GameState(*gameEngine.previousState_)}, map_{new Map(*gameEngine.map_)} {
    commandProcessor_ = gameEngine.commandProcessor_;
    players_ = new std::vector(*gameEngine.players_);
    deck_ = new Deck(*gameEngine.deck_);
}

/**
 * Destructor
 */
GameEngine::~GameEngine() {
    delete state_;
    delete previousState_;
    delete map_;
    // Delete each object in vector
    for (const auto p: *players_) {
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
GameEngine& GameEngine::operator=(const GameEngine&gameEngine) {
    if (this != &gameEngine) {
        // This is used in case we are re-assigning to ourselves i.e. f = f;
        // We need to clean up the current pointers because destructor won't be called.
        delete state_;
        delete previousState_;
        delete map_;
        for (const auto p: *players_) {
            delete p;
        }
        players_->clear();
        delete players_;
        delete deck_;
        state_ = new GameState(*gameEngine.state_);
        players_ = new std::vector(*gameEngine.players_);
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
GameState GameEngine::state() const {
    return *state_;
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
std::vector<Player *>& GameEngine::getPlayers() const {
    return *players_;
}

/**
 * Gets the Deck
 */
Deck& GameEngine::getDeck() const {
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
    for (const auto p: *players_) {
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
    while (*state_ == GameState::win) {
        Command* command = &commandProcessor_->getCommand(*this);
        command->execute();
    }
}

std::string GameEngine::stringToLog() const {
    std::stringstream s;
    s << "| Game Engine new state: " << *this;
    if (*state_ == GameState::win)
        s << " player " << players_->at(0) << " wins!";
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

/**
 * Executes players orders
 */
void GameEngine::executeOrdersPhase() const {
    if (*state_ != GameState::executeOrders)
        return; // Game engine is in the wrong state

    // execute player orders
    for (const auto player: *players_) {
        if (player->getTerritories().empty()) // if player has no territories skip turn
            continue;
        player->orderList().executeOrders();
        player->removeNegotiators();
    }
}

/**
 * Check if game is won
 */
void GameEngine::checkWinningCondition() {
    // Only 1 player left the game is over
    if (*state_ != GameState::executeOrders)
        return; // Game engine is in the wrong state
    if (players_->size() == 1) {
        transition(GameState::win);
    }

    transition(GameState::assignReinforcements);
}

/**
 * Removes eliminated players
 */
void GameEngine::removeEliminatedPlayers() const {
    // remove players which have no territories
    for (auto it = players_->begin(); it != players_->end(); ++it) {
        if ((*it)->getTerritories().empty()) {
            delete *it;
            players_->erase(it);
            --it;
        }
    }
}

/**
 * Executes the reinforcement phase
 */
void GameEngine::reinforcementPhase() {
    if (*state_ != GameState::assignReinforcements)
        return;

    for (const auto player: *players_) {
        if (*previousState_ != GameState::playersAdded) player->fillReinforcementPool();
        player->issueOrders();
        player->orderList().executeOrders();
    }

    transition(GameState::issueOrders);
}

/**
 * Executes the issue orders phase
 */
void GameEngine::issuingOrderPhase()
{
    if (*state_ != GameState::issueOrders)
        return;

    std::cout << "Begin issuing orders" << std::endl;

    while (std::ranges::count_if(*players_, [](const Player *p) { return p->isIssuingOrders(); }) > 0)
    {
        for (const auto player : *players_) {
            // Only ask players who are still issuing orders
            if (player->isIssuingOrders()) {
                player->issueOrder();
            }
        }
    }

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

AddPlayerCommand::AddPlayerCommand(GameEngine&gameEngine, const std::string&playerName) : Command(gameEngine,
    "AddPlayer"), playerName_{new std::string(playerName)} {
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
    if (gameEngine_->getPlayers().size() >= 6) {
        saveEffect("Maximum of 6 players already reached.");
        return false;
    }
    return Command::validate();
}

void AddPlayerCommand::execute() {
    if (!validate()) return;
    gameEngine_->getPlayers().push_back(new Player(*gameEngine_, *playerName_));
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
    if (gameEngine_->state() == GameState::playersAdded && gameEngine_->getPlayers().size() >= 2)
        return true;
    std::stringstream s;
    s << "Command \"" << *description_ << "\" not valid in state \"" << *gameEngine_ << "\".";
    if (gameEngine_->state() == GameState::playersAdded && gameEngine_->getPlayers().size() < 2)
        s << " You need at least 2 players.";
    saveEffect(s.str());
    return false;
}

void GameStartCommand::execute() {
    if (!validate()) return;

    std::stringstream os;

    // create a copy
    std::vector<Territory *> territories{gameEngine_->map().territories()};
    assignTerritories(territories, os);

    setPlayerTurnOrder(os);

    // This lets each player draw 2 cards
    Deck ourDeck = gameEngine_->getDeck();
    for (const auto i: gameEngine_->getPlayers()) {
        i->draw();
        i->draw();
        os << "\n" << i->getName() << " drew " << i->getHand();
    }
    saveEffect(os.str());
    gameEngine_->transition(GameState::assignReinforcements);
}

void GameStartCommand::setPlayerTurnOrder(std::ostream &os) const {
    std::random_device r;
    std::default_random_engine e(r());
    std::ranges::shuffle(gameEngine_->getPlayers(), e);
    os << "Turn Order: ";
    for (const auto p: gameEngine_->getPlayers()) {
        os << p->getName() << " ";
    }
}

void GameStartCommand::assignTerritories(std::vector<Territory *> &territories, std::ostream&os) const {
    os << "Assigned Territories:" << std::endl;
    while (!territories.empty()) {
        for (const auto &it: gameEngine_->getPlayers()) {
            Random rnd; const auto random = rnd.generate(0, territories.size() - 1);
            const auto territory = territories[random];
            it->add(*territory);
            territories.erase(territories.begin() + random);
            os << "\t" << territory->name() << " to " << it->getName() << std::endl;
            if (territories.empty())
                break;
        }
    }
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

Random::Random() {
    e = std::default_random_engine(r());
}

unsigned Random::generate(const unsigned from, const unsigned to) {
    std::uniform_int_distribution u(from, to);
    return u(e);
}
