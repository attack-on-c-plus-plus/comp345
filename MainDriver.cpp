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
#include <cstring>
#include "CommandProcessing.h"
#include "GameEngine.h"
#include "drivers/Drivers.h"

int main(const int argc, char* argv[]) {
    //std::cout << ((strcmp(argv[1], "-file") == 0 && (argc == 2 || argc > 3)) || argc > 2) << std::endl;

    if (argc == 2 && strcmp(argv[1], "-test") == 0) {
        std::cout << "COMP 345" << std::endl;

       testLoadMaps();

        testPlayer();

       testOrderList();

        testCards();

        testLoggingObserver();

        testGameStartup();

        testOrderExecution();

        testReinforcementPhase();
    } else if (argc == 2 && strcmp(argv[1], "-console") == 0) {
        CommandProcessor commandProcessor;
        GameEngine engine = GameEngine(commandProcessor);
        engine.gameLoop();
    } else if (argc == 3 && strcmp(argv[1], "-file") == 0) {
        FileCommandProcessorAdapter fileCommandProcessorAdapter{argv[2]};
        GameEngine engine = GameEngine(fileCommandProcessorAdapter);
        engine.gameLoop();
    } else {
        std::cout << "Please provide a parameter to run the application." << std::endl;
        std::cout << "\t\"" << argv[0] << " -test\" to run the tests." << std::endl;
        std::cout << "\t\"" << argv[0] << " -console\" to run the game in console mode." << std::endl;
        std::cout << "\t\"" << argv[0] << " -file <filename>\" to run the game in file mode." << std::endl;
    }

    return EXIT_SUCCESS;
}
