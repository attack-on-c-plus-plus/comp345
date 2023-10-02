#include <iostream>
#include <string>
#include <vector>
#include "Player.h"

int Player::playerCount = 1;

    //Default Constructor
    Player::Player()
        :name{"Player " + std::to_string(playerCount)}
        {
            playerCount++;
            std::cout << "* In the default constructor of player class * \n";
        }

    //Parameterized Constructor
    Player::Player(std::string n)
        :name{n}
        {
            std::cout << "* In the Parameterized constructor of player class *\n";
            playerCount++;
        }

    Player::Player(const Player& other)
        : name(other.name)
        {
            std::cout << "* In the copy constructor of player class *\n";
            playerCount++;
        }

    Player::~Player()
    {
        std::cout << "Default Destructor is called\n";
        playerCount--;
    }

    std::string Player:: getName()
    {
        return name;
    }

    void Player::changeName(std::string newName)
    {
        name=newName;
    }


    void Player:: toAttack()
    {

    }

    void Player:: toDefend()
    {

    }

    void Player:: issueOrder(std::string order)
    {

    }


