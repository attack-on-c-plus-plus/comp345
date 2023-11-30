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


void AggressivePlayerStrategy::issueOrder(){

    // STEP 1: DEPLOY ALL TO STRONGEST OWNED TERRITORY : we will get the strongest terry of the player using the todefend function where it has the strongest terry at index 0
    // STEP 2: ONCE DEPLOYABLE TROOPS ARE EMPTY, ADVANCE FROM STRONGEST TERRITORY TO ANY ADJACENT ENEMY TERRITORY: using the adjacent check from advanceorder.cpp, we can
    // check first if the target is adjacent to strongest terry defined at step 1 and then advance
    // WHERE SOURCE TERRY = STRONGEST TERRY AND TARGET TERRY = ANY ENEMY ADJACENT TERRY where source armies > then target armies (since an aggressive player's goal is to get the most territories)
    // We will simulate and attack for each owned terry from todefend list on each adjacent enemy territory from toAttack list whilst double checking that the target is adjacent to source


    std::vector<const Territory *> territoriesToAttack = toAttack();
    Territory weakestTerritoryToAttack = *territoriesToAttack[0];

    std::vector<const Territory *> territoriesToDefend = toDefend();
    Territory strongestTerritory = *territoriesToDefend[0];

    std::vector<const Territory *> territoriesToAdvance;

    unsigned int deployableArmies = player_->availableReinforcements();
    unsigned int armiesAvailable;
    unsigned int armiesSent;
    unsigned int armiesDefending;

    //Deploy all armies to the strongest territory
    //checks if player owns a territory first
    if(!territoriesToDefend.empty())
    {
        //then checks if there are any armies left to deploy
        if(deployableArmies>0)
        {
            //Create Deploy order where all armies left are deployed to the strongest territory
            player_->orderList().addOrder(DeployOrder(*gameEngine_, *player_, strongestTerritory, deployableArmies));
            //TODO: Do we have to remove these armies from reinforcement pool? it seems to be handled in availableReinforcements in player.cpp

        }
        else{
            std::cout << " Player doesn't currently have any armies to deploy on strongest territory..." << std::endl;
        }
    }
    else{
        std::cout << " Player doesn't currently own any territories... " << std::endl;
    }




    //Advance armies from strongest territory to adjacent enemy territories
    for (const auto myTerritory: territoriesToAttack) {
        // Find which of these territories in the attack list are adjacent to the strongest territory
        for (auto territories = gameEngine_->map().adjacencies(*myTerritory); auto territory: territories)
        {
            if (territory == territoriesToDefend[0])
            {
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

    for(int i = 0; i< territoriesToDefend.size(); i++) {
        while(true) {
            //Advance armies from strongest territory to adjacent enemy territories
            for (const auto myTerritory: territoriesToAttack) {
                // Find which of these territories in the attack list are adjacent to the strongest territory
                for (auto territories = gameEngine_->map().adjacencies(*myTerritory); auto territory: territories) {
                    if (territory == territoriesToDefend[i]) {
                        territoriesToAdvance.push_back(territory);
                    }
                }
            }
            //THIS IMPLEMENTATION MAKES IT SO THE STRONGEST TERRITORY ADVACNES ON WEAKEST ENEMY TERRITORY AND THEN THE SECOND STRONGEST ADVANCES ON SECOND WEAKEST, ETC...
            // WE WANT STRONGEST ATTACKS ALL ENEMY TERRITORIES ONE BY ONE?
            // OR DO WE WANT STRONGEST TERRITORY ATTACKS STRONGEST ENEMY TERRITORY?

            if ((territoriesToDefend[i]->armyCount() > 0) && (territoriesToAdvance[i]->owner() != *player_) && (Territory::compStrongestArmies(territoriesToDefend[i], territoriesToAttack[i])))
            {
                Territory terry1 = *territoriesToDefend[i];
                Territory terry2 = *territoriesToAttack[i];
                player_->orderList().addOrder(AdvanceOrder(*gameEngine_, *player_, terry1, terry2, territoriesToAttack[i]->armyCount()));

            }
            else{
                break;
            }

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







std::vector<const Territory *> AggressivePlayerStrategy::toDefend() const{
   //toDefend holds all user owned territories for an aggressive player in descending order of army count

    //territories owned by player
    std::vector< const Territory *> territoriesToDefend_StrongestFirst;
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
