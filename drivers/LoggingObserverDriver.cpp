// Daniel Soldera
// Carson Senthilkumar
// Joe El-Khoury
// Henri Stephane Carbon
// Haris Mahmood

#include <iostream>
#include <type_traits>
#include "../LoggingObserver.h"
#include "../Orders.h"
#include "../GameEngine.h"

void testLoggingObserver() {
    Player player1{"Joe"};
    Player player2{"Jane"};
    Territory territory1{"t1"};
    Territory territory2{"t2"};
    Territory territory3{"t3"};
    GameEngine engine{GameState::start};
    LogObserver logObserver{"gamelog.txt"};
    engine.attach(logObserver);

    OrdersList ordersList{};
    ordersList.attach(logObserver);
    std::cout << std::boolalpha;
    std::cout << "Command subclass of Subject: " << std::is_base_of_v<Subject,Command> << std::endl;
    std::cout << "Command subclass of ILoggable: " << std::is_base_of_v<ILoggable,Command> << std::endl;
    AdvanceOrder advanceOrder{player1, territory1, territory2, 5, engine};
    advanceOrder.attach(logObserver);
    ordersList.addOrder(advanceOrder);
    std::cout << "AdvanceOrder subclass of Subject: " << std::is_base_of_v<Subject,AdvanceOrder> << std::endl;
    std::cout << "AdvanceOrder subclass of ILoggable: " << std::is_base_of_v<ILoggable,AdvanceOrder> << std::endl;
    AirliftOrder airliftOrder{player1, territory1, territory2, 4, engine};
    airliftOrder.attach(logObserver);
    ordersList.addOrder(airliftOrder);
    std::cout << "AirliftOrder subclass of Subject: " << std::is_base_of_v<Subject,AirliftOrder> << std::endl;
    std::cout << "AirliftOrder subclass of ILoggable: " << std::is_base_of_v<ILoggable,AirliftOrder> << std::endl;
    BlockadeOrder blockadeOrder{player1, territory2, engine};
    blockadeOrder.attach(logObserver);
    ordersList.addOrder(blockadeOrder);
    std::cout << "BlockadeOrder subclass of Subject: " << std::is_base_of_v<Subject,BlockadeOrder> << std::endl;
    std::cout << "BlockadeOrder subclass of ILoggable: " << std::is_base_of_v<ILoggable,BlockadeOrder> << std::endl;
    BombOrder bombOrder{player1, territory3, engine};
    bombOrder.attach(logObserver);
    ordersList.addOrder(bombOrder);
    std::cout << "BombOrder subclass of Subject: " << std::is_base_of_v<Subject,BombOrder> << std::endl;
    std::cout << "BombOrder subclass of ILoggable: " << std::is_base_of_v<ILoggable,BombOrder> << std::endl;
    DeployOrder deployOrder{player1, territory1, 4, engine};
    deployOrder.attach(logObserver);
    ordersList.addOrder(deployOrder);
    std::cout << "DeployOrder subclass of Subject: " << std::is_base_of_v<Subject,DeployOrder> << std::endl;
    std::cout << "DeployOrder subclass of ILoggable: " << std::is_base_of_v<ILoggable,DeployOrder> << std::endl;
    NegotiateOrder negotiateOrder{player1, player2, engine};
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

    AddPlayerCommand addPlayerCommand{engine, "Bob"};
    addPlayerCommand.attach(logObserver);
    addPlayerCommand.validate();

    AssignTerritoriesCommand assignTerritoriesCommand{engine};
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

    CommandProcessor commandProcessor;

    commandProcessor.attach(logObserver);
    Command *command = &commandProcessor.getCommand(engine);
    command->attach(logObserver);
    command->execute();
}