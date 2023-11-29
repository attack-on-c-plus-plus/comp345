//
// Created by hsc on 25/11/23.
//

#include "TournamentDriver.h"

#include <iostream>

#include "../CommandProcessing.h"
#include "../GameEngine.h"
#include "../PlayerStrategies.h"

/**
 * \brief Tests the Tournament command
 */
void testTournament() {
    const std::string seperator(70, '=');
    std::cout << seperator << std::endl;
    std::cout << "Testing Tournament Command..." << std::endl;
    std::cout << seperator << std::endl << std::endl;

    const FakeRandom random;
    CommandProcessor commandProcessor;
    GameEngine engine{commandProcessor, random};

    std::vector<std::string> maps;
    std::set<Strategy> strategies;

    std::cout << seperator << std::endl;
    std::cout << "Test no maps" << std::endl;
    std::cout << seperator << std::endl;
    TournamentCommand t1{engine, maps, strategies, 0, 0};
    t1.execute();

    maps.emplace_back("maps/valid/Simple.map");

    std::cout << seperator << std::endl;
    std::cout << "Test no strategies" << std::endl;
    std::cout << seperator << std::endl;
    TournamentCommand t2{engine, maps, strategies, 0, 0};
    t2.execute();

    strategies.insert(Strategy::Neutral);
    strategies.insert(Strategy::Cheater);

    std::cout << seperator << std::endl;
    std::cout << "Test no games" << std::endl;
    std::cout << seperator << std::endl;
    TournamentCommand t3{engine, maps, strategies, 0, 0};
    t3.execute();

    std::cout << seperator << std::endl;
    std::cout << "Test no too little turns" << std::endl;
    std::cout << seperator << std::endl;
    TournamentCommand t4{engine, maps, strategies, 1, 9};
    t4.execute();

    // Add invalid strategy
    strategies.insert(Strategy::Human);

    std::cout << seperator << std::endl;
    std::cout << "Invalid player strategy" << std::endl;
    std::cout << seperator << std::endl;
    TournamentCommand t5{engine, maps, strategies, 0, 0};
    t5.execute();

    // Remove invalid strategy
    strategies.erase(Strategy::Human);

    // Put engine in invalid state
    engine.transition(GameState::mapLoaded);

    std::cout << seperator << std::endl;
    std::cout << "Test invalid state" << std::endl;
    std::cout << seperator << std::endl;

    TournamentCommand t6{engine, maps, strategies, 1, 10};
    t6.execute();

    // Put engine in valid state
    engine.transition(GameState::start);

    maps.emplace_back("maps/valid/3D.map");
    std::cout << seperator << std::endl;
    std::cout << "Test valid tournament" << std::endl;
    std::cout << seperator << std::endl;
    TournamentCommand t7{engine, maps, strategies, 2, 10};
    t7.execute();
}
