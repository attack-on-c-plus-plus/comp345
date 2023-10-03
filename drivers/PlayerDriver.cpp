//
// Created by Haris Mahmood on 2023-09-27.
//
#include <iostream>
#include "../Player.h"

using namespace std;

void testplayer () {

    //Object with pointer created on the heap (Manually have to delete)
    Player *player1 = new Player("Bob");

    cout << "Player 1 name is: "<<player1->getName()<<"\n\n";
    delete player1;

    Player player2;
    Player *player3=new Player(player2);
    cout << "Player 3 name is: "<<player3->getName()<<"\n\n";
    delete player3;


//    Player *player2 = new Player(*player1);
//    cout << "Player 2 name is: "<<player2->getName()<<"\n\n";
//    delete player2;
//
//    Player *player3 = new Player("s");
//    cout <<"Player 3 name is: "<<player3->getName()<<"\n";
//    delete player3;         // To delete a object
//
//    //Object without pointer created on the stack (Automatically deletes)
//    Player player4;
//    cout << "Player 4 name is: "<<player4.getName()<<"\n";

};
