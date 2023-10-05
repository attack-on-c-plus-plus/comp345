#include <iostream>
#include <string>
#include <vector>
#include "Player.h"
//#include "Map.h"
//#include "OrdersList"

    //Default Constructor
    Player::Player()
    :name{new std::string("Player Default")}
        {
            std::cout << "* In the default constructor of player class * \n";
        }

    //Parameterized Constructor
    Player::Player(const std::string &name)
        :name{new std::string(name)}
        {
            std::cout << "* In the Parameterized constructor of player class *\n";
        }

    //Copy Constructor
    Player::Player(const Player& other)
            :name{new std::string(*(other.name))}
        {
            std::cout << "* In the copy constructor of player class *\n";
        }

    Player::~Player()
    {
        delete name;
        std::cout << "Default Destructor is called\n";
    }

    std::string Player:: getName() const
    {
        return *name;
    }

    void Player::changeName(const std::string &newName)
    {
        name = new std::string(newName);
    }


    // Add a method to add a territory to the player's collection
    void Player:: addTerritory(const Territory &territory)
    {
        territories.push_back(territory);
    }

    bool Player::removeTerritory(const Territory &territoryToRemove)
    {

    }


    std::vector<Territory> Player::toDefend()
    {
        std::vector<Territory> territoriesToDefend;

        // Iterate through the player's territories
        for (const Territory &territory : territories) {
            // Get the adjacent territories of the current territory
            std::vector<Territory> adjacentTerritories = territory.getAdjacencies();

            // Check if any adjacent territory is not owned by the player
            bool needsDefense = false;
            for (const Territory &adjacentTerritory : adjacentTerritories) {
                if (adjacentTerritory.getOwner() != this) {
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


    std::vector<Territory> Player:: toAttack()
    {
        std::vector<Territory> territoriesToAttack;

        // Iterate through the player's territories
        for (const Territory &myTerritory : territories)
        {
            // Get the adjacent territories of the current territory
            std::vector<Territory> adjacentTerritories = myTerritory.getAdjacencies();

            // Check if adjacent territories are owned by other players
            for (const Territory &adjacentTerritory : adjacentTerritories)
            {
                if (adjacentTerritory.getOwner() != this) {
                    // Add the adjacent territory to the list of territories to attack
                    territoriesToAttack.push_back(adjacentTerritory);
                }
            }
        }

        return territoriesToAttack;
    }


    void Player::issueOrder(const std::string &orderType, int sourceTerritory, int targetTerritory, int armies, int targetPlayerID)
    {
        Order *newOrder = nullptr;

        if (orderType == "Deploy")
        {
            // Create a DeployOrder object
            newOrder = new DeployOrder(armies);
        }

        else if (orderType == "Advance")
        {
            // Create an AdvanceOrder object
            newOrder = new AdvanceOrder(sourceTerritory, targetTerritory, armies);
        }

        else if (orderType == "Bomb")
        {
            // Create a BombOrder object
            newOrder = new BombOrder(targetTerritory);
        }

        else if (orderType == "Blockade")
        {
            // Create a BlockadeOrder object
            newOrder = new BlockadeOrder(targetTerritory);
        }

        else if (orderType == "Airlift")
        {
            // Create an AirliftOrder object
            newOrder = new AirliftOrder(armies, sourceTerritory, targetTerritory);
        }

        else if (orderType == "Negotiate")
        {
            // Create a NegotiateOrder object with the target player ID
            newOrder = new NegotiateOrder(targetPlayerID);
        }

        // Add the new order to the player's orders list
        if (newOrder != nullptr)
        {
            ordersList.addOrder(newOrder);
        }
    }

    void Player::drawCardFromDeck(Deck& deck)
    {
        Card drawnCard = deck.draw();
        if (drawnCard.getCardType() != "")
        {
            hand.addCard(drawnCard);
        }
    }

    void Player:: playCardFromHand(const Card& card, Deck& deck)
    {
        // Implement logic to play the card
        card.play();
        hand.removeCard(card);
    }

    const Hand& Player:: getHand() const
    {
        return hand;
    }






