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

#include <cstring>
#include <iostream>
#include <cstring>
#include "CommandProcessing.h"
#include "GameEngine.h"

#include "drivers/CardsDriver.h"
#include "drivers/CommandProcessingDriver.h"
#include "drivers/GameEngineDriver.h"
#include "drivers/LoggingObserverDriver.h"
#include "drivers/MapDriver.h"
#include "drivers/OrdersDriver.h"
#include "drivers/PlayerDriver.h"
#include "drivers/TournamentDriver.h"
#include "drivers/PlayerStrategiesDriver.h"

int main(const int argc, char* argv[]) {
    if ((argc == 2 || argc == 3) && strcmp(argv[1], "-test") == 0) {
        if (argc == 2 || strcmp(argv[2], "all") == 0 || strcmp(argv[2], "map") == 0)
            testLoadMaps();
        if (argc == 2 || strcmp(argv[2], "all") == 0 || strcmp(argv[2], "player") == 0) {
            //testPlayer();
            testPlayers();
        }
        if (argc == 2 || strcmp(argv[2], "all") == 0 || strcmp(argv[2], "order") == 0) {
            testOrderList();
            testOrderExecution();
        }
        if (argc == 2 || strcmp(argv[2], "all") == 0 || strcmp(argv[2], "card") == 0)
            testCards();
        if (argc == 2 || strcmp(argv[2], "all") == 0 || strcmp(argv[2], "command") == 0)
            testCommandProcessor();
        if (argc == 2 || strcmp(argv[2], "all") == 0 || strcmp(argv[2], "logging") == 0)
            testLoggingObserver();
        if (argc == 2 || strcmp(argv[2], "all") == 0 || strcmp(argv[2], "gamestartup") == 0) {
            testGameStartup();
        }
        if (argc == 2 || strcmp(argv[2], "all") == 0 || strcmp(argv[2], "gamemainloop") == 0) {
            testMainGameLoop();
        }
        if (argc == 2 || strcmp(argv[2], "all") == 0 || strcmp(argv[2], "tournament") == 0) {
            testTournament();
        }
         if (argc == 2 || strcmp(argv[2], "all") == 0 || strcmp(argv[2], "playerStrategies") == 0) {
            testPlayerStrategies();
        }
    } else if (argc == 2 && strcmp(argv[1], "-console") == 0) {
        CommandProcessor commandProcessor;
        const Random random;
        GameEngine engine = GameEngine(commandProcessor, random);
        engine.gameLoop();
    } else if (argc == 3 && strcmp(argv[1], "-file") == 0) {
        FileCommandProcessorAdapter fileCommandProcessorAdapter{argv[2]};
        const Random random;
        GameEngine engine = GameEngine(fileCommandProcessorAdapter, random);
        engine.gameLoop();
    } else {
        std::cout << "Please provide a parameter to run the application." << std::endl;
        std::cout << "\t\"" << argv[0] << " -test\" to run the tests." << std::endl;
        std::cout << "\t\"" << argv[0] << " -console\" to run the game in console mode." << std::endl;
        std::cout << "\t\"" << argv[0] << " -file <filename>\" to run the game in file mode." << std::endl;
    }

    return EXIT_SUCCESS;
}
