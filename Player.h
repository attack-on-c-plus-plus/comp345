//
// Created by Haris Mahmood on 2023-09-27.
//

#ifndef COMP345_PLAYER_H
#define COMP345_PLAYER_H
#include <string>


class Player {
    private:
        //Attributes
        std::string name;
        static int playerCount;

    public:
        //Constructor
        Player();
        Player(std::string name);
        Player(const Player& other);

        //Destructor
        ~Player();

        //Methods
        std::string getName();      //Extra
        void changeName(std::string newName);
        void toAttack();
        void toDefend();
        void issueOrder(std::string order);
};
#endif //COMP345_PLAYER_H
