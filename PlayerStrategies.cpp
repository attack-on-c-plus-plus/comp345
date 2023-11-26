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
    int i = 0;
    int index = -1;
    int armies = 100;

    for (auto territory : targets)
    {
        int deployable = player_->availableReinforcements();
        if (territory->armyCount() <= armies)
        {
            armies = territory->armyCount();
            Territory terry = *territory;
            if (deployable > 10 && deployable != 0)
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
    }

    // Card Orders
    const std::vector<Card *> myHand = player_->hand().cards();

    bool playNegotiate, playAirlift = false;

    for (auto card : myHand)
    {
        if (card->type() == CardType::airlift)
        {
            playAirlift = true;
        }
        if (card->type() == CardType::diplomacy)
        {
            playNegotiate = true;
        }
    }

    if (playAirlift)
    {
        player_->orderList().addOrder(AirliftOrder(gameEngine_, player_, ));
    }

    if (playNegotiate)
    {
        player_->orderList().addOrder(NegotiateOrder(gameEngine_, player_, ));
    }
    // Add an end order, to signify the end of orders
    player_->orderList().addOrder(EndOrder(*gameEngine_, *player_));
    return;
}

/**
 * \brief Gives the territories that can be attacked
 * \return the territories to attack
 */
std::vector<const Territory *> BenevolentPlayerStrategy::toAttack() const
{
    return {};
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
    return territoriesToDefend;
}