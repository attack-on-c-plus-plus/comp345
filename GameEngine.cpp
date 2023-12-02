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
#include "Player.h"
#include "PlayerStrategies.h"

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
    maxTurns_ = new std::optional<unsigned>();
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
    delete maxTurns_;
}
constexpr int width = 12;
    constexpr int mapWidth = 20;

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
        gameOverPhase();
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
    delete deck_;
    delete map_;
    delete players_;
    map_ = new Map();
    players_ = new Players(*this);
    deck_ = new Deck(*random_, 24);
}

void GameEngine::mainGameLoop() {
    while (*state_ != GameState::win) {
        reinforcementPhase();
        issuingOrderPhase();
        executeOrdersPhase();
    }
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
    if (*state_ == GameState::win) {
        players_->size() == 1 ? s << " player " << *players_ << "wins!" : s << *players_ << "draw";
    }
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

std::optional<unsigned> GameEngine::maxTurns() const {
    return *maxTurns_;
}

void GameEngine::maxTurns(const unsigned turns) const {
    *maxTurns_ = turns;
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

AddPlayerCommand::AddPlayerCommand(plconst AddPlayerCommand&addPlayer) = default;

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
    if (const Player p(*gameEngine_, *playerName_, *strategy_); !gameEngine_->players().add(p)) {
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

TournamentCommand::TournamentCommand(GameEngine& gameEngine, const std::vector<std::string> &maps,
    const std::set<Strategy> &players, const unsigned games, const unsigned rounds) :
    Command(gameEngine, "Tournament"), mapFiles_{new std::vector(maps)},
    strategies_{new std::set(players)}, games_{new unsigned(games)}, turns_{new unsigned(rounds)} {
}

TournamentCommand::TournamentCommand(const TournamentCommand& tournament)  : Command(tournament),
    mapFiles_{new std::vector(*tournament.mapFiles_)}, strategies_{new std::set(*tournament.strategies_)},
    games_{new unsigned(*tournament.games_)}, turns_{new unsigned(*tournament.turns_)} {

}

TournamentCommand::~TournamentCommand() {
    delete mapFiles_;
    delete strategies_;
    delete games_;
    delete turns_;
}

bool TournamentCommand::validate() {
    if (mapFiles_->size() > 5 || mapFiles_->empty()) {
        saveEffect("Invalid number of maps: " + std::to_string(mapFiles_->size()) + ". Only 1 to 5 maps can be used.");
        return false;
    }
    if (strategies_->contains(Strategy::Human)) {
        saveEffect("Invalid strategy: " + toString(Strategy::Human) + ". Only computer players strategies allowed.");
        return false;
    }
    if (strategies_->size() < 2 || strategies_->size() > 4) {
        saveEffect("Invalid number of computer players strategies: " + std::to_string(strategies_->size()) + ". Only 2 to 4 strategies can be used.");
        return false;
    }
    if (*games_ < 1 || *games_ > 5) {
        saveEffect("Invalid number of games: " + std::to_string(*games_) + ". Only 1 to 5 games can be played on each map.");
        return false;
    }
    if (*turns_ < 10 || *turns_ > 50) {
        saveEffect("Invalid number of turns: " + std::to_string(*turns_) + ". Only 10 to 50 turns for each game.");
        return false;
    }
    if (gameEngine_->state() == GameState::start && *games_ > 0 && *turns_ > 0 && !mapFiles_->empty() && strategies_->size() >= 2) {
        return true;
    }
    return Command::validate();
}

std::string TournamentCommand::generateResults(const std::vector<std::vector<std::string>> &winners,
    const std::vector<std::string> &maps) const {
    std::stringstream ss;
    ss << "Tournament Results";
    createTableHeader(ss, *games_);
    createTable(ss, maps, winners, *games_);
    return ss.str();
}

void TournamentCommand::execute() {
    if (!validate()) return;
    saveEffect("Running a tournament! (" + std::to_string(mapFiles_->size()) + " maps, " +
        std::to_string(strategies_->size()) + " computer strategies, " + std::to_string(*games_) +
        " games per map, " + std::to_string(*turns_) + " rounds per game maximum)");

    // set max turns for engine
    gameEngine_->maxTurns(*turns_);

    std::vector<std::vector<std::string>> winners;
    std::vector<std::string> maps;

    // LoadMap and validate
    for (int i = 0; i < mapFiles_->size(); ++i) {
        std::vector<std::string> mapWinners;
        winners.push_back(mapWinners);
        // loop games
        for (int j = 0; j < *games_; ++j) {
            gameEngine_->resetGameElements();
            LoadMapCommand loadMap{*gameEngine_, mapFiles_->at(i)};
            if (!loadMap.validate()) {
                saveEffect("Tournament failed! Bad map file \"" + mapFiles_->at(i) + "\"");
                return;pl
            }
            loadMap.execute();
            ValidateMapCommand validateMap{*gameEngine_};
            if (!validateMap.validate()) {
                saveEffect("Tournament failed! Bad map \"" + gameEngine_->map().name() + "\"");
                return;
            }
            validateMap.execute();

            // add players
            for (const auto strategy: *strategies_) {
                AddPlayerCommand addPlayer{*gameEngine_, toString(strategy), strategy};
                addPlayer.execute();
            }
            GameStartCommand start{*gameEngine_};
            start.execute();

            // start game loop
            gameEngine_->mainGameLoop();

            // determine the winner
            winners[i].push_back(gameEngine_->players().winner());

            ReplayCommand replay{*gameEngine_};
            replay.execute();
        }
        maps.push_back(gameEngine_->map().name());
    }
    saveEffect(generateResults(winners, maps));
}

TournamentCommand& TournamentCommand::operator=(const TournamentCommand& command) {
    if (this != &command) {
        Command::operator=(command);
        delete mapFiles_;
        delete strategies_;
        delete games_;
        delete turns_;
        mapFiles_ = new std::vector(*command.mapFiles_);
        strategies_ = new std::set(*command.strategies_);
        games_ = new unsigned(*command.games_);
        turns_ = new unsigned(*command.turns_);
    }
    return *this;
}

void TournamentCommand::createTable(std::stringstream&ss, const std::vector<std::string>&maps,
                                    const std::vector<std::vector<std::string>>&winners, const unsigned games) {
    for (int i = 0; i < winners.size(); ++i) {
        ss << std::endl;
        ss << "|" << std::right << std::setw(mapNameWidth) << std::setfill(' ') << (maps[i] + " ") << "|";
        for (const auto& winner: winners[i]) {
            ss << std::left << std::setw(strategyWidth) << std::setfill(' ') << (" " + winner) << "|";
        }
        createRowDelimiter(ss, games);
    }
}

void TournamentCommand::createTableHeader(std::stringstream&ss, const unsigned games) {
    createRowDelimiter(ss, games);
    ss << std::endl << "|" << std::setw(mapNameWidth) << std::setfill(' ') << " " << "|";
    for (int i = 1; i <= games; ++i)
        ss << std::left << std::setw(strategyWidth) << std::setfill(' ') << (" Game " + std::to_string(i)) << "|";
    createRowDelimiter(ss, games);
}

void TournamentCommand::createRowDelimiter(std::stringstream&ss, const unsigned games) {
    // Create row delimeter
    ss  << std::endl << "+" << std::string(mapNameWidth, '-') << "+";
    for (int i = 0; i < games; ++i)
        ss << std::string(strategyWidth, '-') << "+";
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
    const std::default_random_engine e(r());
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
    const std::mt19937 e{123};
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
