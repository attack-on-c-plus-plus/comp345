// Daniel Soldera
// Carson Senthilkumar
// Joe El-Khoury
// Henri Stephane Carbon
// Haris Mahmood
#include <iostream>

#include "drivers/Drivers.h"

int main() {
    std::cout << "COMP 345" << std::endl;

    testLoadMaps();

    testPlayer();

    testOrderList();

    testCards();

    testLoggingObserver();

    testGameStates();

    return 0;
}
