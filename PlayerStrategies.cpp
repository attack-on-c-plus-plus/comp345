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

#include "PlayerStrategies.h"

#include "CommandProcessing.h"
#include "GameEngine.h"
#include "Map.h"
#include "Orders.h"
#include "Player.h"
#include <utility>
#include <algorithm>

/**
 * \brief Constructor
 * \param player
 * \param gameEngine
 */
PlayerStrategy::PlayerStrategy(Player& player, GameEngine& gameEngine) {
    player_ = &player;
    gameEngine_ = &gameEngine;
}

/**
 * \brief Copy Constructor
 * \param playerStrategy
 */
PlayerStrategy::PlayerStrategy(const PlayerStrategy& playerStrategy) {
    player_ = playerStrategy.player_;
    gameEngine_ = playerStrategy.gameEngine_;
}

/**
 * \brief Destructor
 */
PlayerStrategy::~PlayerStrategy() = default;

/**
 * \brief operator= overload
 * \param playerStrategy
 * \return the assigned player strategy
 */
PlayerStrategy& PlayerStrategy::operator=(const PlayerStrategy& playerStrategy) {
    if (this != &playerStrategy) {
        // no need to delete player_
        player_ = playerStrategy.player_;
    }
    return *this;
}

/**
 * \brief Constructor
 * \param player
 * \param gameEngine
 */
HumanPlayerStrategy::HumanPlayerStrategy(Player& player, GameEngine& gameEngine) : PlayerStrategy(player, gameEngine) {
    commandProcessor_ = new CommandProcessor();
}

/**
 * \brief Copy Constructor
 * \param humanPlayerStrategy
 */
HumanPlayerStrategy::HumanPlayerStrategy(const HumanPlayerStrategy& humanPlayerStrategy) : PlayerStrategy(humanPlayerStrategy) {
    commandProcessor_ = new CommandProcessor(*humanPlayerStrategy.commandProcessor_);
}

/**
 * \brief Destructor
 */
HumanPlayerStrategy::~HumanPlayerStrategy() {
    delete commandProcessor_;
}

/**
 * \brief operator= overload
 * \param humanPlayerStrategy
 * \return the assigned player strategy
 */
HumanPlayerStrategy& HumanPlayerStrategy::operator=(const HumanPlayerStrategy& humanPlayerStrategy) {
    if (this != &humanPlayerStrategy) {
        delete commandProcessor_;
        PlayerStrategy::operator=(humanPlayerStrategy);
        commandProcessor_ = new CommandProcessor(*humanPlayerStrategy.commandProcessor_);
    }
    return *this;
}

/**
 * \brief Issues an order
 */
void HumanPlayerStrategy::issueOrder() {
    if (player_->orderList().done()) return;
    commandProcessor_->getOrder(*gameEngine_, *player_);
}

/**
 * \brief Gives the territories that can be attacked
 * \return the territories to attack
 */
std::vector<const Territory*> HumanPlayerStrategy::toAttack() const {
    std::vector<const Territory*> territoriesToAttack;

    // Iterate through the player's territories
    for (const auto myTerritory: player_->territories()) {
        // Check if adjacent territories are owned by other players
        for (auto territories = gameEngine_->map().adjacencies(*myTerritory); auto territory: territories) {
            if (&territory->owner() != player_) {
                // Check that owner is not on cantAttack list because of negotiate order
                bool canAttack = true;
                for (const auto player : player_->cantAttack())
                {
                    if (&territory->owner() == &*player)
                    {
                        canAttack = false;
                        break;
                    }
                }

                // Check if adjacentTerritory is not already in territoriesToAttack
                bool isUnique = true;
                for (const auto attackTerritory : territoriesToAttack) {
                    if (attackTerritory->name() == territory->name()) {
                        isUnique = false;
                        break;
                    }
                }
                // If it's unique, add it to the list of territories to attack
                if (isUnique && canAttack)
                {
                    territoriesToAttack.push_back(territory);
                }
            }
        }
    }
    return territoriesToAttack;
}

/**
 * \brief Gives the territories that need to be defended
 * \return the territories to defend
 */
std::vector<const Territory*> HumanPlayerStrategy::toDefend() const {
    std::vector<const Territory *> territoriesToDefend;

    // Iterate through the player's territories
    for (const auto territory: player_->territories()) {
        // Get the adjacent territories of the current territory
        auto adjacentTerritories = gameEngine_->map().adjacencies(*territory);

        // Check if any adjacent territory is not owned by the player
        bool needsDefense = false;
        for (const auto adjacentTerritory: adjacentTerritories) {
            if (adjacentTerritory->owner().name() != player_->name()) {
                needsDefense = true;
                break; // At least one adjacent territory is not owned by the player
            }
        }

        if (needsDefense) {
            // Add the current territory to the list of territories to defend
            territoriesToDefend.push_back(territory);
        }
    }

    return territoriesToDefend;
}

/**
 * \brief Constructor
 * \param player
 * \param gameEngine
 */
NeutralPlayerStrategy::NeutralPlayerStrategy(Player& player, GameEngine& gameEngine) : PlayerStrategy(player, gameEngine) {
}

/**
 * \brief Copy Constructor
 * \param neutralPlayerStrategy
 */
NeutralPlayerStrategy::NeutralPlayerStrategy(const NeutralPlayerStrategy& neutralPlayerStrategy) = default;

NeutralPlayerStrategy& NeutralPlayerStrategy::operator=(const NeutralPlayerStrategy& neutralPlayerStrategy) {
    if (this == &neutralPlayerStrategy) {
        PlayerStrategy::operator=(neutralPlayerStrategy);
    }
    return *this;
}

/**
 * \brief Issues an order
 */
void NeutralPlayerStrategy::issueOrder() {
    // Add an end order, Neutral player does not issue orders
    player_->orderList().addOrder(EndOrder(*gameEngine_, *player_));
}

/**
 * \brief Gives the territories that can be attacked
 * \return the territories to attack
 */
std::vector<const Territory*> NeutralPlayerStrategy::toAttack() const {
    return {};
}

/**
 * \brief Gives the territories that need to be defended
 * \return the territories to defend
 */
std::vector<const Territory*> NeutralPlayerStrategy::toDefend() const {
    return {};
}

std::string toString(const Strategy strategy) {
    switch (strategy) {
        case Strategy::Neutral: return "Neutral";
        case Strategy::Cheater: return "Cheater";
        case Strategy::Human: return "Human";
        case Strategy::Aggressive: return "Aggressive";
        case Strategy::Benevolent: return "Benevolent";
    }
    return "";
}

/**
 * \brief Constructor
 * \param player
 * \param gameEngine
 */
BenevolentPlayerStrategy::BenevolentPlayerStrategy(Player &player, GameEngine &gameEngine) : PlayerStrategy(player, gameEngine)
{
}

/**
 * \brief Copy Constructor
 * \param benevolentPlayerStrategy
 */
BenevolentPlayerStrategy::BenevolentPlayerStrategy(const BenevolentPlayerStrategy &benevolentPlayerStrategy) = default;

BenevolentPlayerStrategy &BenevolentPlayerStrategy::operator=(const BenevolentPlayerStrategy &benevolentPlayerStrategy)
{
    if (this == &benevolentPlayerStrategy)
    {
        PlayerStrategy::operator=(benevolentPlayerStrategy);
    }
    return *this;
}

/**
 * \brief Issues an order
 */
void BenevolentPlayerStrategy::issueOrder()
{
    std::vector<const Territory *> targets = toDefend();

    for (auto territory : targets)
    {
        unsigned int deployable = player_->availableReinforcements();

            Territory terry = *territory;
            if (deployable > 10)
            {
                unsigned deploy = 10;

                player_->orderList().addOrder(DeployOrder(*gameEngine_, *player_, terry, deploy));
            }
            else
            {
                if (deployable > 0)
                {
                    player_->orderList().addOrder(DeployOrder(*gameEngine_, *player_, terry, player_->availableReinforcements()));
                }
            }
    }

    // Card Orders
    const std::vector<Card *> myHand = player_->hand().cards();

    bool playNegotiate = false;

    for (auto card : myHand)
    {
        if (card->type() == CardType::diplomacy)
        {
            playNegotiate = true;
        }
    }

    auto attacker = toAttack();

    if (playNegotiate)
    {
        player_->orderList().addOrder(NegotiateOrder(*gameEngine_, *player_, attacker[0]->owner()));
    }
    // Add an end order, to signify the end of orders
    player_->orderList().addOrder(EndOrder(*gameEngine_, *player_));
}

/**
 * \brief Gives the territories that can be attacked
 * \return the territories to attack
 */
std::vector<const Territory *> BenevolentPlayerStrategy::toAttack() const
{
    std::vector<const Territory *> territoriesToAttack;

    // Iterate through the player's territories
    for (const auto myTerritory : player_->territories())
    {
        // Check if adjacent territories are owned by other players
        for (auto territories = gameEngine_->map().adjacencies(*myTerritory); auto territory : territories)
        {
            if (&territory->owner() != player_)
            {
                // Check that owner is not on cantAttack list because of negotiate order
                bool canAttack = true;
                for (const auto player : player_->cantAttack())
                {
                    if (&territory->owner() == &*player)
                    {
                        canAttack = false;
                        break;
                    }
                }

                // Check if adjacentTerritory is not already in territoriesToAttack
                bool isUnique = true;
                for (const auto attackTerritory : territoriesToAttack)
                {
                    if (attackTerritory->name() == territory->name())
                    {
                        isUnique = false;
                        break;
                    }
                }
                // If it's unique, add it to the list of territories to attack
                if (isUnique && canAttack)
                {
                    territoriesToAttack.push_back(territory);
                }
            }
        }
    }
    std::sort(territoriesToAttack.begin(), territoriesToAttack.end(), [](const Territory *a, const Territory *b)
              { return a->armyCount() > b->armyCount(); });
    return territoriesToAttack;
}

/**
 * \brief Gives the territories that need to be defended
 * \return the territories to defend
 */
std::vector<const Territory *> BenevolentPlayerStrategy::toDefend() const
{
    std::vector<const Territory *> territoriesToDefend;

    // Iterate through the player's territories
    for (const auto territory : player_->territories())
    {
        // Get the adjacent territories of the current territory
        auto adjacentTerritories = gameEngine_->map().adjacencies(*territory);

        // Check if any adjacent territory is not owned by the player
        bool needsDefense = false;
        for (const auto adjacentTerritory : adjacentTerritories)
        {
            if (adjacentTerritory->owner().name() != player_->name())
            {
                needsDefense = true;
                break; // At least one adjacent territory is not owned by the player
            }
        }

        if (needsDefense)
        {
            // Add the current territory to the list of territories to defend
            territoriesToDefend.push_back(territory);
        }
    }
    std::sort(territoriesToDefend.begin(), territoriesToDefend.end(), [](const Territory *a, const Territory *b)
              { return a->armyCount() < b->armyCount(); });
    return territoriesToDefend;
}

CheaterPlayerStrategy::CheaterPlayerStrategy(Player &player, GameEngine &gameEngine) : PlayerStrategy(player,gameEngine) {}

CheaterPlayerStrategy &CheaterPlayerStrategy::operator=(const CheaterPlayerStrategy &cheaterPlayerStrategy) {
    if (this == &cheaterPlayerStrategy) {
        PlayerStrategy::operator=(cheaterPlayerStrategy);
    }
    return *this;
}

/**
 * Returns the territories to defend for the Cheater player
 * @return A vector of pointers to territories
 */
std::vector<const Territory *> CheaterPlayerStrategy::toDefend() const {
    std::vector<const Territory *> territoriesToDefend;
    for (const auto territory : player_->territories()) {
        auto adjacentTerritories = gameEngine_->map().adjacencies(*territory);
        bool toDefend = false;
        for (const auto adjacentTerritory : adjacentTerritories) {
            if (adjacentTerritory->owner().name() != player_->name()) {
                toDefend = true;
                break;
            }
        }
        if (toDefend) {
            territoriesToDefend.push_back(territory);
        }
    }
    return territoriesToDefend;
}

/**
 * Returns the territories to attack for the Cheater player
 * @return A vector of pointers to territories
 */
std::vector<const Territory *> CheaterPlayerStrategy::toAttack() const {
    std::vector<const Territory *> territoriesToAttack;
    for (const auto territory : player_->territories()) {
        auto adjacentTerritories = gameEngine_->map().adjacencies(*territory);
        for (const auto adjacentTerritory : adjacentTerritories) {
            if (adjacentTerritory->owner() != *player_) {
                bool toAttack = true;
                for (const auto player : player_->cantAttack()) {
                    if (adjacentTerritory->owner() == *player) {
                        toAttack = false;
                        break;
                    }
                }
                bool isUnique = true;
                for (const auto attackTerritory : adjacentTerritories) {
                    if (attackTerritory->name() == territory->name()) {
                        isUnique = false;
                        break;
                    }
                }
                if (isUnique && toAttack) {
                    territoriesToAttack.push_back(adjacentTerritory);
                }
            }
        }
    }
    return territoriesToAttack;
}

/**
 * The Cheater player automatically conquers all territories adjacent to him, 1 per turn
 */
void CheaterPlayerStrategy::issueOrder() {
    // The cheater would, logically, try to select the territory with the
    // most adjacent territories not belonging to him.
    std::vector<const Territory *> territoriesToAttack = toAttack();

    // If there are no territories left to attack, then return
    if (territoriesToAttack.empty()) {
        return;
    }

    std::vector<std::pair<int, const Territory *>> adjacentCounter;
    for (auto territory : territoriesToAttack) {
        // The game checks for adjacent territories of adjacent territories
        auto adjacentTerritories = gameEngine_->map().adjacencies(*territory);
        int nbOfAdjacentTerr = 0;
        for (auto adjacentTerritory : adjacentTerritories) {
            // if the territory does not belong to the player, then add +1 to the counter
            if (adjacentTerritory->name() != player_->name()) {
                nbOfAdjacentTerr++;
            }
        }
        adjacentCounter.emplace_back(nbOfAdjacentTerr, territory);
    }
    // Here, we check what is the territory that must be attacked in priority
    // The one with the highest number of adjacent territories is prioritized

    auto maxElement =
            std::max_element(adjacentCounter.begin(), adjacentCounter.end(), comparePairs);

    // Find the source territory of the player to advance their soldiers
    Territory *srcTerry;
    for (Territory * terry : player_->territories()) {
        auto adjacentTerries = gameEngine_->map().adjacencies(*terry);
        for (auto adjacentTerry : adjacentTerries) {
            if (adjacentTerry == maxElement->second) {
                srcTerry = terry;
                break;
            }
        }
    }

    player_->orderList().addOrder(BombOrder(*gameEngine_, *player_, *maxElement->second));
    player_->orderList().addOrder(AdvanceOrder(*gameEngine_, *player_,
                                               *srcTerry, *maxElement->second, srcTerry->armyCount()/2));

}

CheaterPlayerStrategy::CheaterPlayerStrategy(const CheaterPlayerStrategy &cheaterPlayerStrategy) = default;

/**
 * Function to compare pairs of <int, const Territory*> based on the first
 * @param lhs : Integer
 * @param rhs : A pointer to a constant territory
 * @return If the lhs.first is less than rhs.first
 */
bool comparePairs(const std::pair<int, const Territory*>& lhs, const std::pair<int, const Territory*>& rhs) {
    return lhs.first < rhs.first;
}