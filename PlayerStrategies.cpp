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
std::vector<Territory*> HumanPlayerStrategy::toDefend() const {
    std::vector<Territory *> territoriesToDefend;

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
    if (gameEngine_->state() == GameState::assignReinforcements) {
        // Deploy troops
        const auto toDeploy = player_->availableReinforcements() / player_->territories().size();
        auto remainder = player_->availableReinforcements() % player_->territories().size();

        for (const auto territory: player_->territories()) {
            player_->orderList().addOrder(DeployOrder(*gameEngine_, *player_, *territory, toDeploy + remainder));
            if (remainder > 0) remainder--;
        }
        player_->orderList().addOrder(EndOrder(*gameEngine_, *player_));
    } else {
        // Add an end order, Neutral player does not issue orders
        player_->orderList().addOrder(EndOrder(*gameEngine_, *player_));
    }
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
std::vector<Territory*> NeutralPlayerStrategy::toDefend() const {
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
    if (gameEngine_->state() == GameState::assignReinforcements) {
        // Deploy troops
        const auto toDeploy = player_->availableReinforcements() / player_->toDefend().size();
        auto remainder = player_->availableReinforcements() % player_->toDefend().size();

        for (const auto territory: player_->toDefend()) {
            player_->orderList().addOrder(DeployOrder(*gameEngine_, *player_, *territory, toDeploy + remainder));
            if (remainder > 0) remainder--;
            if (toDeploy == 0 && remainder == 0) break;
        }
        player_->orderList().addOrder(EndOrder(*gameEngine_, *player_));
    } else {
        // Card Orders
        const std::vector<Card *> myHand = player_->hand().cards();

        bool playNegotiate = false;

        for (const auto card : myHand)
        {
            if (card->type() == CardType::diplomacy)
            {
                playNegotiate = true;
            }
        }

        const auto attacker = toAttack();

        if (playNegotiate)
        {
            player_->orderList().addOrder(NegotiateOrder(*gameEngine_, *player_, attacker[0]->owner()));
        }
        // Add an end order, to signify the end of orders
        player_->orderList().addOrder(EndOrder(*gameEngine_, *player_));
    }
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
    std::ranges::sort(territoriesToAttack, [](const Territory *a, const Territory *b)
                      { return a->armyCount() > b->armyCount(); });
    return territoriesToAttack;
}

/**
 * \brief Gives the territories that need to be defended
 * \return the territories to defend
 */
std::vector<Territory *> BenevolentPlayerStrategy::toDefend() const
{
    std::vector<Territory *> territoriesToDefend;

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
