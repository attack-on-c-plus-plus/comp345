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

#include <iostream>

#include "drivers/Drivers.h"

int main() {
    std::cout << "COMP 345" << std::endl;

    testLoadMaps();

    testPlayer();

    testOrderList();

    testCards();

    testLoggingObserver();

    testGameStartup();

    testReinforcementPhase();

    return 0;
}
