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

#include "Player.h"

#include <algorithm>
#include <ranges>
#include <string>
#include <vector>

#include "Cards.h"
#include "CommandProcessing.h"
#include "GameEngine.h"
#include "Map.h"
#include "Orders.h"
#include "PlayerStrategies.h"

/**
 * Parameterized Constructor
 */
Player::Player(GameEngine&gameEngine, const std::string&name, const Strategy strategy) : name_{new std::string(name)},
    territories_{new std::vector<Territory *>}, ordersList_{new OrdersList()},
    cantTarget_{new std::vector<const Player *>}, reinforcementPool_{new unsigned(50)}, deployComplete_{new bool(false)},
    ordersComplete_{new bool(false)}, hand_{new Hand(*this)}, strategy_{new Strategy(strategy)} {
    gameEngine_ = &gameEngine;
    playerStrategy_ = createStrategy(strategy);
}

/**
 * Copy Constructor
 */
Player::Player(const Player& player) : name_{new std::string(*player.name_)}, territories_{new std::vector(*player.territories_)},
                                      ordersList_{new OrdersList(*player.ordersList_)}, cantTarget_{new std::vector(*player.cantTarget_)},
                                      reinforcementPool_{new unsigned(*player.reinforcementPool_)},
                                      deployComplete_{new bool(*player.deployComplete_)},
                                      ordersComplete_{new bool(*player.ordersComplete_)},
                                      strategy_{new Strategy(*player.strategy_)} {
    hand_ = new Hand(*player.hand_);
    gameEngine_ = player.gameEngine_;
    playerStrategy_ = createStrategy(*player.strategy_);
}

/**
 * Destructor
 */
Player::~Player() {
    delete name_;
    delete ordersList_;
    delete hand_;
    delete territories_;
    delete reinforcementPool_;
    delete cantTarget_;
    delete deployComplete_;
    delete ordersComplete_;
    delete playerStrategy_;
    delete strategy_;
}

Player& Player::operator=(const Player& player) {
    if (this != &player) {
        // This is used in case we are re-assigning to ourselves i.e. f = f;
        // We need to clean up the current pointers because destructor won't be called.
        delete name_;
        delete ordersList_;
        delete hand_;
        delete territories_;
        delete reinforcementPool_;
        delete cantTarget_;
        delete deployComplete_;
        delete ordersComplete_;
        delete playerStrategy_;
        delete strategy_;
        name_ = new std::string(*player.name_);
        ordersList_ = new OrdersList(*player.ordersList_);
        territories_ = new std::vector(*player.territories_);
        reinforcementPool_ = new unsigned(*player.reinforcementPool_);
        cantTarget_ = new std::vector(*player.cantTarget_);
        hand_ = new Hand(*player.hand_);
        deployComplete_ = new bool(*player.deployComplete_);
        ordersComplete_ = new bool(*player.ordersComplete_);
        gameEngine_ = player.gameEngine_;
        strategy_ = new Strategy(*player.strategy_);
        playerStrategy_ = createStrategy(*player.strategy_);
    }
    return *this;
}

bool Player::operator==(const Player&player) const {
    if (this == &player || *name_ == player.name())
        return true;
    return false;
}

std::string Player::name() const {
    return *name_;
}

void Player::name(const std::string& newName) {
    name_ = new std::string(newName);
}

unsigned Player::reinforcementPool() const {
    return *reinforcementPool_;
}

Strategy Player::strategy() const {
    return *strategy_;
}

OrdersList& Player::orderList() const {
    return *ordersList_;
}

const Hand& Player::hand() const {
    return *hand_;
}

const GameEngine& Player::gameEngine() const {
    return *gameEngine_;
}

const std::vector<Territory *>& Player::territories() const {
    return *territories_;
}

std::vector<const Territory *> Player::toAttack() const {
    return playerStrategy_->toAttack();
}

std::vector<const Territory *> Player::toDefend() const {
    return playerStrategy_->toDefend();
}

const std::vector<const Player *>& Player::cantAttack() const {
    return *cantTarget_;
}

/**
 * \brief Issues multiple orders
 */
void Player::issueOrders() const {
    // get the command from the command line
    while (!orderList().done()) {
        issueOrder();
    }
}

/**
 * \brief Issues one order
 */
void Player::issueOrder() const {
    if (orderList().done()) return;
    playerStrategy_->issueOrder();
}

/**
 * \brief Add a territory to the player's collection
 * \param territory
 */
void Player::add(Territory&territory) const {
    territories_->push_back(&territory);
    territory.owner(*this);
}

void Player::remove(const Territory& territory) const {
    if (const auto it = std::ranges::find(*territories_, &territory); it != territories_->end())
        territories_->erase(it);
}

void Player::addNegotiator(const Player& negotiator) const {
    cantTarget_->push_back(&negotiator);
}

void Player::removeNegotiators() const {
    cantTarget_->clear();
}

void Player::draw() const {
    gameEngine_->deck().draw(*hand_);
}

void Player::play(const CardType cardType, std::istream &is) const {
    // Get the card in the hand
    auto hasType = [cardType](const Card* card) { return card->type() == cardType; };
    if (const auto iterator = std::ranges::find_if(hand_->cards(), hasType); iterator != hand_->cards().end()) {
        gameEngine_->deck().discard((*iterator)->play(is), *hand_);
    }
}

void Player::fillReinforcementPool() const {
    *reinforcementPool_ += territoryBonusArmies() + continentBonusArmies();
}

void Player::deploy(const unsigned armies) const {
    if (armies > *reinforcementPool_)
        throw std::out_of_range("armies cannot be larger than pool of reinforcements");
    *reinforcementPool_ -= armies;
}

unsigned Player::availableReinforcements() const {
    auto deployed = [](const int accumulator, const DeployOrder* d) {
        return d->armies() > d->player().reinforcementPool() ? accumulator : accumulator + d->armies();
    };
    auto isDeployOrder = [](const Order *d) { return dynamic_cast<const DeployOrder *>(d) != nullptr; };
    auto deploy = [](const Order *o) { return dynamic_cast<const DeployOrder *>(o); };
    auto filtered = ordersList_->orders() | std::views::filter(isDeployOrder) | std::views::transform(deploy);
    const int used = std::accumulate(filtered.begin(), filtered.end(), 0, deployed);
    return used <= reinforcementPool() ? reinforcementPool() - used : 0;
}

unsigned int Player::continentBonusArmies() const {
    unsigned armies = 0;
    const Map&map = gameEngine_->map();
    const auto&p = *this;
    auto ownedBy = [&p](const Territory* t) { return t->owner() == p; };
    for (const auto continent: map.continents()) {
        if (auto adj = map.adjacencies(*continent); adj.size() == std::ranges::count_if(adj, ownedBy))
            armies += continent->bonusArmies();
    }
    return armies;
}

unsigned int Player::territoryBonusArmies() const {
    return territories_->size() >= 9 ? territories_->size() / 3 : 3;
}

PlayerStrategy* Player::createStrategy(const Strategy strategy) {
    PlayerStrategy* playerStrategy = nullptr;
    switch (strategy) {
        case Strategy::Neutral:
            playerStrategy = new NeutralPlayerStrategy(*this, *gameEngine_);
            break;
        case Strategy::Cheater:
            // TODO: Add cheater
            playerStrategy = new NeutralPlayerStrategy(*this, *gameEngine_);
            break;
        case Strategy::Human:
            playerStrategy = new HumanPlayerStrategy(*this, *gameEngine_);
            break;
        case Strategy::Aggressive:
            playerStrategy = new AggressivePlayerStrategy(*this, *gameEngine_);
            break;
        case Strategy::Benevolent:
            playerStrategy = new BenevolentPlayerStrategy(*this, *gameEngine_);
            break;
    }
    return playerStrategy;
}

std::ostream& operator<<(std::ostream&os, const Player& player) {
    os << *player.name_;
    return os;
}

Players::Players(GameEngine& gameEngine) : players_(new std::vector<Player*>()) {
    gameEngine_ = &gameEngine;
    currentTurn_ = new unsigned(1);
    players_->reserve(20); // reserve extra so it does shift in memory
}

Players::~Players() {
    for (const auto p : *players_)
        delete p;
    delete players_;
    delete currentTurn_;
}

void Players::setPlayOrder() const {
    gameEngine_->random().setPlayOrder(*players_);
}

/**
 * \brief Adds a player if no player with the same name exist
 * \param player the player to add
 * \return true if added; false otherwise
 */
bool Players::add(const Player &player) const {
    // find is player exist with same name
    if (const auto it = std::ranges::find_if(*players_, [player](const Player* p) { return player.name() == p->name(); }); it != players_->end())
        return false;

    players_->push_back(new Player(*gameEngine_, player.name(), player.strategy()));
    return true;
}

Player& Players::neutral() const {
    auto is_neutral = [](const Player* p) { return p->strategy() == Strategy::Neutral; };

    if (const auto it = std::ranges::find_if(*players_, is_neutral); it != players_->end())
        return **it;

    const auto neutral = new Player(*gameEngine_, "Neutral", Strategy::Neutral);
    players_->push_back(neutral);
    return *neutral;
}

bool Players::has(const Player& player) const {
    return std::ranges::find(*players_, &player) != players_->end();
}

Player& Players::player(const unsigned id) const {
    return *players_->at(id);
}

void Players::issueOrders() const {
    while (std::ranges::count_if(*players_, [](const Player *p) { return !p->orderList().done(); }) > 0)
    {
        for (const auto player : *players_) {
            // Only ask players who are still issuing orders
            if (!player->orderList().done()) {
                gameEngine_->map().print();
                player->issueOrder();
            }
        }
    }
}

void Players::executeOrders() const {
    for (auto player_iterator = players_->begin(); player_iterator != players_->end(); ++player_iterator) {
        (*player_iterator)->orderList().executeOrders();
        (*player_iterator)->removeNegotiators();
        if ((*player_iterator)->territories().empty()) {
            // player has no more territories remove them from the game
            delete *player_iterator;
            players_->erase(player_iterator);
            --player_iterator;
        }
    }
    // If one player is left, they win
    if (players_->size() == 1 || (gameEngine_->maxTurns().has_value() && *currentTurn_ == gameEngine_->maxTurns().value()))
        gameEngine_->transition(GameState::win);
    // Order execution complete, go back to deployment phase
    else
        gameEngine_->transition(GameState::assignReinforcements);
    (*currentTurn_)++;
}

void Players::reinforcement() const {
    for (const auto player: *players_) {
        if (gameEngine_->previousState() != GameState::playersAdded) player->fillReinforcementPool();
        player->issueOrders();
        player->orderList().executeOrders();
    }
}

bool Players::hasMinimum() const {
    return players_->size() >= 2;
}

bool Players::hasMaximum() const {
    return players_->size() >= 6;
}

void Players::init(std::ostream&os) const {
    // create a copy
    std::vector territories{gameEngine_->map().territories()};
    os << "Assigned Territories:" << std::endl;
    while (!territories.empty()) {
        for (const auto &it: *players_) {
            const auto random = gameEngine_->random().generate(0, territories.size() - 1);
            const auto territory = territories[random];
            it->add(*territory);
            territories.erase(territories.begin() + random);
            os << "\t" << territory->name() << " to " << it->name() << std::endl;
            if (territories.empty())
                break;
        }
    }

    gameEngine_->players().setPlayOrder();
    os << "Turn Order: ";
    for (const auto p: *players_) {
        os << p->name() << " ";
    }

    // This lets each player draw 2 cards
    for (const auto i: *players_) {
        i->draw();
        i->draw();
        os << "\n" << i->name() << " drew " << i->hand();
    }
}

size_t Players::size() const {
    return players_->size();
}

std::string Players::winner() const {
    if (players_->size() == 1) return players_->at(0)->name();
    auto players = *players_;
    // Sort players in descending order by territory owned
    std::ranges::sort(players,
        [](const Player *a, const Player *b){ return a->territories().size() > b->territories().size(); });

    // if top two players have same amount, we have a draw
    if (players.at(0)->territories().size() == players.at(1)->territories().size())
        return "Draw";

    // otherwise top player is the winner
    return players.at(0)->name();
}

std::ostream& operator<<(std::ostream&os, const Players& players) {
    for (const auto player: *players.players_) {
        os << *player << " ";
    }
    return os;
}
