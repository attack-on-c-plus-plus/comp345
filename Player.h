//
// Created by Haris Mahmood on 2023-09-27.
//

#ifndef COMP345_PLAYER_H
#define COMP345_PLAYER_H
#include <string>

class Player {
    private:
        //Attributes
        std::string *name;


    public:
        //Constructor
        Player();
        Player(const std::string &name);
        Player(const Player& other);

        //Destructor
        ~Player();

        //Methods
        std::string getName() const;      //Extra
        void changeName(const std::string &newName);
        void toAttack();
        void toDefend();
        void issueOrder(const std::string &order);

};
#endif //COMP345_PLAYER_H

