//
// Created by hsc on 15/11/23.
//

#include "PlayerStrategies.h"

#include "CommandProcessing.h"
#include "GameEngine.h"
#include "Map.h"
#include "Player.h"

PlayerStrategy::PlayerStrategy(Player& player, GameEngine& gameEngine) {
    player_ = &player;
    gameEngine_ = &gameEngine;
}

PlayerStrategy::PlayerStrategy(const PlayerStrategy& playerStrategy) {
    player_ = playerStrategy.player_;
    gameEngine_ = playerStrategy.gameEngine_;
}

PlayerStrategy::~PlayerStrategy() = default;

PlayerStrategy& PlayerStrategy::operator=(const PlayerStrategy& playerStrategy) {
    if (this != &playerStrategy) {
        // no need to delete player_
        player_ = playerStrategy.player_;
    }
    return *this;
}

HumanPlayerStrategy::HumanPlayerStrategy(Player& player, GameEngine& gameEngine) : PlayerStrategy(player, gameEngine) {
    commandProcessor_ = new CommandProcessor();
}

HumanPlayerStrategy::HumanPlayerStrategy(const HumanPlayerStrategy& humanPlayerStrategy) : PlayerStrategy(humanPlayerStrategy) {
    commandProcessor_ = new CommandProcessor(*humanPlayerStrategy.commandProcessor_);
}

HumanPlayerStrategy::~HumanPlayerStrategy() {
    delete commandProcessor_;
}

HumanPlayerStrategy& HumanPlayerStrategy::operator=(const HumanPlayerStrategy& humanPlayerStrategy) {
    if (this != &humanPlayerStrategy) {
        delete commandProcessor_;
        PlayerStrategy::operator=(humanPlayerStrategy);
        commandProcessor_ = new CommandProcessor(*humanPlayerStrategy.commandProcessor_);
    }
    return *this;
}

void HumanPlayerStrategy::issueOrder() {
    if (!(player_->isDeploying() || player_->isIssuingOrders())) return;

    if (const Order *order = commandProcessor_->getOrder(*gameEngine_, *player_)) {
        player_->orderList().addOrder(*order);
        return;
    }
    player_->doneOrders();
}

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

NeutralPlayerStrategy::NeutralPlayerStrategy(Player& player, GameEngine& gameEngine) : PlayerStrategy(player, gameEngine) {
}

NeutralPlayerStrategy::NeutralPlayerStrategy(const NeutralPlayerStrategy& neutralPlayerStrategy) = default;

NeutralPlayerStrategy& NeutralPlayerStrategy::operator=(const NeutralPlayerStrategy& neutralPlayerStrategy) {
    if (this == &neutralPlayerStrategy) {
        PlayerStrategy::operator=(neutralPlayerStrategy);
    }
    return *this;
}

void NeutralPlayerStrategy::issueOrder() {
    player_->doneOrders();
}

std::vector<const Territory*> NeutralPlayerStrategy::toAttack() const {
    return {};
}

std::vector<const Territory*> NeutralPlayerStrategy::toDefend() const {
    return {};
}

