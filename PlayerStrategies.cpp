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
            player_->orderList().addOrder(DeployOrder(*gameEngine_, *player_, *territory, toDeploy + (remainder > 0 ? 1 : 0)));
            if (remainder > 0) remainder--;
            if (toDeploy == 0 && remainder == 0) break;
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
            player_->orderList().addOrder(DeployOrder(*gameEngine_, *player_, *territory, toDeploy + (remainder > 0) ? 1 : 0));
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
std::vector<Territory *> CheaterPlayerStrategy::toDefend() const {
    std::vector<Territory *> territoriesToDefend;
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
    for (auto territory : player_->territories()) {
        auto adjacentTerritories = gameEngine_->map().adjacencies(*territory);
        for (auto adjacentTerritory : adjacentTerritories) {
            if (adjacentTerritory->owner() != *player_) {
                bool toAttack = true;
                for (auto player : player_->cantAttack()) {
                    if (adjacentTerritory->owner() == *player) {
                        toAttack = false;
                        break;
                    }
                }
                bool isUnique = true;
                for (auto attackTerritory : adjacentTerritories) {
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
    if (gameEngine_->state() == GameState::assignReinforcements) {
        // Deploy troops
        const auto toDeploy = player_->availableReinforcements() / player_->toDefend().size();
        auto remainder = player_->availableReinforcements() % player_->toDefend().size();

        for (const auto territory: player_->toDefend()) {
            player_->orderList().addOrder(DeployOrder(*gameEngine_, *player_, *territory, toDeploy + (remainder > 0 ? 1 : 0)));
            if (remainder > 0) remainder--;
            if (toDeploy == 0 && remainder == 0) break;
        }
        player_->orderList().addOrder(EndOrder(*gameEngine_, *player_));
    } else {
        // The cheater would, logically, try to select the territory with the
        // most adjacent territories not belonging to him.
        const std::vector<const Territory *> territoriesToAttack = toAttack();

        // If there are no territories left to attack, then return
        if (territoriesToAttack.empty()) {
            return;
        }

        std::vector<std::pair<int, const Territory *>> adjacentCounter;
        for (const Territory * territory : territoriesToAttack) {
            // The game checks for adjacent territories of adjacent territories
            auto adjacentTerritories = gameEngine_->map().adjacencies(*territory);
            int nbOfAdjacentTerr = 0;
            for (const Territory * adjacentTerritory : adjacentTerritories) {
                // if the territory does not belong to the player, then add +1 to the counter
                if (adjacentTerritory->name() != player_->name()) {
                    nbOfAdjacentTerr++;
                }
            }
            adjacentCounter.emplace_back(nbOfAdjacentTerr, territory);
        }
        // Here, we check what is the territory that must be attacked in priority
        // The one with the highest number of adjacent territories is prioritized
        const auto maxElement = std::ranges::max_element(adjacentCounter, comparePairs);

        for (const auto terryToAttack = maxElement->second; const auto terry : gameEngine_->map().adjacencies(*terryToAttack)) {
            if (terry->owner() != *player_) {
                terry->removeArmies(terry->armyCount());
                terry->owner().remove(*terry);
                player_->add(*terry);
            }
        }

        player_->orderList().addOrder(EndOrder(*gameEngine_, *player_));
    }
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


/**
 * \brief Constructor
 * \param player
 * \param gameEngine
 */
AggressivePlayerStrategy::AggressivePlayerStrategy(Player &player, GameEngine &gameEngine) : PlayerStrategy(player, gameEngine)
{
}

/**
 * \brief Copy Constructor
 * \param benevolentPlayerStrategy
 */
AggressivePlayerStrategy::AggressivePlayerStrategy(const AggressivePlayerStrategy &aggressivePlayerStrategy) = default;

/**
 * \brief operator= overload
 * \param AggressivePlayerStrategy
 * \return the assigned player strategy
 */
AggressivePlayerStrategy &AggressivePlayerStrategy::operator=(const AggressivePlayerStrategy &aggressivePlayerStrategy)
{
    if (this == &aggressivePlayerStrategy)
    {
        PlayerStrategy::operator=(aggressivePlayerStrategy);
    }
    return *this;
}


void AggressivePlayerStrategy::issueOrder() {

    // STEP 1: DEPLOY ALL TO STRONGEST OWNED TERRITORY : we will get the strongest terry of the player using the todefend function where it has the strongest terry at index 0
    // STEP 2: ONCE DEPLOYABLE TROOPS ARE EMPTY, ADVANCE FROM STRONGEST TERRITORY TO ANY ADJACENT ENEMY TERRITORY: using the adjacent check from advanceorder.cpp, we can
    // check first if the target is adjacent to strongest terry defined at step 1 and then advance
    // WHERE SOURCE TERRY = STRONGEST TERRY AND TARGET TERRY = ANY ENEMY ADJACENT TERRY where source armies > then target armies (since an aggressive player's goal is to get the most territories)
    // We will simulate and attack for each owned terry from todefend list on each adjacent enemy territory from toAttack list whilst double checking that the target is adjacent to source


    std::vector<const Territory *> territoriesToAttack = toAttack();
    Territory weakestTerritoryToAttack = *territoriesToAttack[0];

    std::vector< Territory *> territoriesToDefend = toDefend();
    Territory strongestTerritory = *territoriesToDefend[0];

    std::vector<const Territory *> territoriesToAdvance;
    std::vector<const Territory *> territoriesToBomb;

    unsigned int deployableArmies = player_->availableReinforcements();
    //Deploy all armies to the strongest territory
    //checks if player owns a territory first
    if (!territoriesToDefend.empty()) {
        //then checks if there are any armies left to deploy
        if (deployableArmies > 0) {
            //Create Deploy order where all armies left are deployed to the strongest territory
            player_->orderList().addOrder(DeployOrder(*gameEngine_, *player_, strongestTerritory, deployableArmies));
            //TODO: Do we have to remove these armies from reinforcement pool? it seems to be handled in availableReinforcements in player.cpp

        } else {
            std::cout << " Player doesn't currently have any armies to deploy on strongest territory..." << std::endl;
        }
    }
    else {
        std::cout << " Player doesn't currently own any territories... " << std::endl;
    }




    //Advance armies from strongest territory to adjacent enemy territories
    for (const auto myTerritory: territoriesToAttack) {
        // Find which of these territories in the attack list are adjacent to the strongest territory
        for (auto territories = gameEngine_->map().adjacencies(*myTerritory); auto territory: territories) {
            if (territory == territoriesToDefend[0]) {
                territoriesToAdvance.push_back(territory);
            }
        }
    }

    // territoriesToAdvance now holds all enemy territories that are adjacent to the user's strongest territory.
    // The list is also sorted by army count in ascending order since territoriesToAttack (aka toAttack) is sorted by army count in ascending order

    //I can now advance from strongest territory to its adjacent enemy territories from easiest enemy to hardest
    // what happens if the strongest territory beats all enemy territories...? the enemy territories of the strongest territory must adapt to this....
    // do I have to create a function that takes a territory and returns all its adjacent enemy territories so that i can call this function for each advance attempt until the advance fails?

    //this is how we can refresh the list of adjacent enemies :

    for (auto &i: territoriesToDefend) {

        //Advance armies from strongest territory to adjacent enemy territories
        for (const auto myTerritory: territoriesToAttack) {
            // Find which of these territories in the attack list are adjacent to the strongest territory
            for (auto territories = gameEngine_->map().adjacencies(*myTerritory); auto territory: territories) {
                if (territory == i) {
                    territoriesToAdvance.push_back(territory);
                }
            }
        }
        //THIS IMPLEMENTATION MAKES IT SO THE STRONGEST TERRITORY ADVACNES ON WEAKEST ENEMY TERRITORY AND THEN THE SECOND STRONGEST ADVANCES ON SECOND WEAKEST, ETC...
        // WE WANT STRONGEST ATTACKS ALL ENEMY TERRITORIES ONE BY ONE?
        // OR DO WE WANT STRONGEST TERRITORY ATTACKS STRONGEST ENEMY TERRITORY?
        //UPDATE: The below for loop makes it so each enemy adjacent territory is attacked if possible
        //Also note: I decided that it would make sense that the number of armies to advance must be greater than number of defending armies so: armies to advance = target armies + (source - target)/2 armies

        for (auto &j: territoriesToAdvance) {
            if ((i->armyCount() > 0) && (j->owner() != *player_) && (Territory::compStrongestArmies(i, j))) {
                Territory source = *i;
                Territory target = *j;
                player_->orderList().addOrder(AdvanceOrder(*gameEngine_, *player_, source, target, ((j->armyCount()) + ((i->armyCount() - j->armyCount()) /2))));
            }
            else
            {
                // remaining territories to bomb
                territoriesToBomb.push_back(j);
            }
        }

    }

    //Now lets play the bomb card on remaining territories if possible
    for (auto &i: territoriesToBomb) {
        if (player_->hand().has(CardType::bomb)) {
            player_->orderList().addOrder(BombOrder(*gameEngine_, *player_, *i));
            //TODO:do we need to know remove the bomb order from the player's hand?
        }
    }


}
std::vector<const Territory *> AggressivePlayerStrategy::toAttack() const {

    std::vector<const Territory *> adjacentTerritoriesToAttack_WeakestFirst;

    // Filling up adjacentTerritoriesToAttack with enemy adjacent territories from weakest to strongest in army size
    // Iterate through the player's territories
    for (const auto myTerritory: player_->territories()) {
        // Check if adjacent territories are owned by other players
        for (auto territories = gameEngine_->map().adjacencies(*myTerritory); auto territory: territories) {
            if (&territory->owner() != player_) {
                // Check that owner is not on cantAttack list because of negotiate order
                bool canAttack = true;
                for (const auto player: player_->cantAttack()) {
                    if (&territory->owner() == &*player) {
                        canAttack = false;
                        break;
                    }
                }

                // Check if adjacentTerritory is not already in territoriesToAttack
                bool isUnique = true;
                for (const auto attackTerritory: adjacentTerritoriesToAttack_WeakestFirst) {
                    if (attackTerritory->name() == territory->name()) {
                        isUnique = false;
                        break;
                    }
                }
                // If it's unique, add it to the list of territories to attack
                if (isUnique && canAttack) {
                    adjacentTerritoriesToAttack_WeakestFirst.push_back(territory);
                }
            }
        }
    }

    // sort all enemy adjacent territories from weakest to strongest in army size
    std::sort(adjacentTerritoriesToAttack_WeakestFirst.begin(), adjacentTerritoriesToAttack_WeakestFirst.end(),Territory::compWeakestArmies);
    return adjacentTerritoriesToAttack_WeakestFirst;

}







std::vector< Territory *> AggressivePlayerStrategy::toDefend() const{
   //toDefend holds all user owned territories for an aggressive player in descending order of army count

    //territories owned by player
    std::vector< Territory *> territoriesToDefend_StrongestFirst;
    std::vector<Territory *> territoriesToDefend = player_->territories();

   if(territoriesToDefend.empty())
   {

       //sort player's territories from strongest to weakest (very useful for the issueOrder phase)
       //sort in descending order of armies done using the comparator defined in map.cpp
       std::stable_sort(territoriesToDefend.begin(), territoriesToDefend.end(), Territory::compStrongestArmies);

       for(auto & i : territoriesToDefend){

           territoriesToDefend_StrongestFirst.push_back(i);
       }

   }
   else
   {
       std::cout << " Player doesn't currently have any territories to defend... " << std::endl;
   }

   return territoriesToDefend_StrongestFirst;
}
