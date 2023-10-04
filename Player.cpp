#include <iostream>
#include <string>
#include <vector>
#include "Player.h"


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


    void Player:: toAttack()
    {

    }

    void Player:: toDefend()
    {

    }

    void Player:: issueOrder(const std::string &order)
    {

    }

    bool operator==(const Player& lhs, const Player& rhs)
    {
        return *lhs.name == *rhs.name;
    }




