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

#include "LoggingObserverDriver.h"

#include <iostream>
#include <type_traits>

#include "../CommandProcessing.h"
#include "../GameEngine.h"
#include "../LoggingObserver.h"
#include "../Map.h"
#include "../Orders.h"
#include "../Player.h"

void createMap(Map &map);

void testLoggingObserver() {
    const std::string seperator(70, '=');
    std::cout << seperator << std::endl;
    std::cout << "Testing Log Observer" << std::endl;
    std::cout << seperator << std::endl;
    FakeRandom random;
    CommandProcessor commandProcessor;
    GameEngine engine{commandProcessor, random};
    Player player1{engine, "Joe", Strategy::Human};
    Player player2{engine, "Jane", Strategy::Human};
    auto &map = engine.map();
    createMap(map);

    player1.add(map.territory(0));
    player1.add(map.territory(1));
    player2.add(map.territory(2));

    LogObserver logObserver{"gamelog.txt"};
    engine.attach(logObserver);

    std::cout << seperator << std::endl;
    std::cout << "Testing Log Observer: Orders" << std::endl;
    std::cout << seperator << std::endl;

    OrdersList ordersList{};
    ordersList.attach(logObserver);
    std::cout << std::boolalpha;
    std::cout << "Command subclass of Subject: " << std::is_base_of_v<Subject,Command> << std::endl;
    std::cout << "Command subclass of ILoggable: " << std::is_base_of_v<ILoggable,Command> << std::endl;
    AdvanceOrder advanceOrder{engine, player1, map.territory(0), map.territory(1), 5};
    advanceOrder.attach(logObserver);
    ordersList.addOrder(advanceOrder);
    std::cout << "AdvanceOrder subclass of Subject: " << std::is_base_of_v<Subject,AdvanceOrder> << std::endl;
    std::cout << "AdvanceOrder subclass of ILoggable: " << std::is_base_of_v<ILoggable,AdvanceOrder> << std::endl;
    AirliftOrder airliftOrder{engine, player1, map.territory(0), map.territory(1), 4};
    airliftOrder.attach(logObserver);
    ordersList.addOrder(airliftOrder);
    std::cout << "AirliftOrder subclass of Subject: " << std::is_base_of_v<Subject,AirliftOrder> << std::endl;
    std::cout << "AirliftOrder subclass of ILoggable: " << std::is_base_of_v<ILoggable,AirliftOrder> << std::endl;
    BlockadeOrder blockadeOrder{engine, player1, map.territory(1)};
    blockadeOrder.attach(logObserver);
    ordersList.addOrder(blockadeOrder);
    std::cout << "BlockadeOrder subclass of Subject: " << std::is_base_of_v<Subject,BlockadeOrder> << std::endl;
    std::cout << "BlockadeOrder subclass of ILoggable: " << std::is_base_of_v<ILoggable,BlockadeOrder> << std::endl;
    BombOrder bombOrder{engine, player1, map.territory(2)};
    bombOrder.attach(logObserver);
    ordersList.addOrder(bombOrder);
    std::cout << "BombOrder subclass of Subject: " << std::is_base_of_v<Subject,BombOrder> << std::endl;
    std::cout << "BombOrder subclass of ILoggable: " << std::is_base_of_v<ILoggable,BombOrder> << std::endl;
    DeployOrder deployOrder{engine, player1, map.territory(0), 4};
    deployOrder.attach(logObserver);
    ordersList.addOrder(deployOrder);
    std::cout << "DeployOrder subclass of Subject: " << std::is_base_of_v<Subject,DeployOrder> << std::endl;
    std::cout << "DeployOrder subclass of ILoggable: " << std::is_base_of_v<ILoggable,DeployOrder> << std::endl;
    NegotiateOrder negotiateOrder{engine, player1, player2};
    negotiateOrder.attach(logObserver);
    ordersList.addOrder(negotiateOrder);
    std::cout << "NegotiateOrder subclass of Subject: " << std::is_base_of_v<Subject,NegotiateOrder> << std::endl;
    std::cout << "NegotiateOrder subclass of ILoggable: " << std::is_base_of_v<ILoggable,NegotiateOrder> << std::endl;

    std::cout << "Order subclass of Subject: " << std::is_base_of_v<Subject,Order> << std::endl;
    std::cout << "Order subclass of ILoggable: " << std::is_base_of_v<ILoggable,Order> << std::endl;

    std::cout << "OrdersList subclass of Subject: " << std::is_base_of_v<Subject,OrdersList> << std::endl;
    std::cout << "OrdersList subclass of ILoggable: " << std::is_base_of_v<ILoggable,OrdersList> << std::endl;

    std::cout << "GameEngine subclass of Subject: " << std::is_base_of_v<Subject,GameEngine> << std::endl;
    std::cout << "GameEngine subclass of ILoggable: " << std::is_base_of_v<ILoggable,GameEngine> << std::endl;

    std::cout << std::noboolalpha;

    ordersList.executeOrders();

    std::cout << seperator << std::endl;
    std::cout << "Testing Log Observer: Commands" << std::endl;
    std::cout << seperator << std::endl;

    // Set state to an invalid state to test save effect for invalid commands
    engine.transition(GameState::mapValidated);

    LoadMapCommand loadMapCommand{engine, ""};
    loadMapCommand.attach(logObserver);
    loadMapCommand.validate();

    // Set state to an invalid state to test save effect for invalid commands
    engine.transition(GameState::start);

    ValidateMapCommand validateMapCommand{engine};
    validateMapCommand.attach(logObserver);
    validateMapCommand.validate();

    AddPlayerCommand addPlayerCommand{engine, "Bob", Strategy::Human};
    addPlayerCommand.attach(logObserver);
    addPlayerCommand.validate();

    GameStartCommand assignTerritoriesCommand{engine};
    assignTerritoriesCommand.attach(logObserver);
    assignTerritoriesCommand.validate();

    ReplayCommand playCommand{engine};
    playCommand.attach(logObserver);
    playCommand.validate();

    QuitCommand quitCommand{engine};
    quitCommand.attach(logObserver);
    quitCommand.validate();

    // test transition change logging
    engine.transition(GameState::mapLoaded);

    // reset state to start to try using a command from the command line
    engine.transition(GameState::start);

    std::cout << seperator << std::endl;
    std::cout << "Testing Log Observer: Command Processor" << std::endl;
    std::cout << seperator << std::endl;

    commandProcessor.attach(logObserver);
    Command *command = &commandProcessor.getCommand(engine);
    command->attach(logObserver);
    command->execute();
}