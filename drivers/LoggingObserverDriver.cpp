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
    AdvanceOrder advanceOrder{player1, territory1, territory2, 5};
    advanceOrder.attach(logObserver);
    ordersList.addOrder(advanceOrder);
    std::cout << "AdvanceOrder subclass of Subject: " << std::is_base_of_v<Subject,AdvanceOrder> << std::endl;
    std::cout << "AdvanceOrder subclass of ILoggable: " << std::is_base_of_v<ILoggable,AdvanceOrder> << std::endl;
    AirliftOrder airliftOrder{player1, territory1, territory2, 4};
    airliftOrder.attach(logObserver);
    ordersList.addOrder(airliftOrder);
    std::cout << "AirliftOrder subclass of Subject: " << std::is_base_of_v<Subject,AirliftOrder> << std::endl;
    std::cout << "AirliftOrder subclass of ILoggable: " << std::is_base_of_v<ILoggable,AirliftOrder> << std::endl;
    BlockadeOrder blockadeOrder{player1, territory2};
    blockadeOrder.attach(logObserver);
    ordersList.addOrder(blockadeOrder);
    std::cout << "BlockadeOrder subclass of Subject: " << std::is_base_of_v<Subject,BlockadeOrder> << std::endl;
    std::cout << "BlockadeOrder subclass of ILoggable: " << std::is_base_of_v<ILoggable,BlockadeOrder> << std::endl;
    BombOrder bombOrder{player1, territory3};
    bombOrder.attach(logObserver);
    ordersList.addOrder(bombOrder);
    std::cout << "BombOrder subclass of Subject: " << std::is_base_of_v<Subject,BombOrder> << std::endl;
    std::cout << "BombOrder subclass of ILoggable: " << std::is_base_of_v<ILoggable,BombOrder> << std::endl;
    DeployOrder deployOrder{player1, territory1, 4};
    deployOrder.attach(logObserver);
    ordersList.addOrder(deployOrder);
    std::cout << "DeployOrder subclass of Subject: " << std::is_base_of_v<Subject,DeployOrder> << std::endl;
    std::cout << "DeployOrder subclass of ILoggable: " << std::is_base_of_v<ILoggable,DeployOrder> << std::endl;
    NegotiateOrder negotiateOrder{player1, player2};
    negotiateOrder.attach(logObserver);
    ordersList.addOrder(negotiateOrder);
    std::cout << "NegotiateOrder subclass of Subject: " << std::is_base_of_v<Subject,NegotiateOrder> << std::endl;
    std::cout << "NegotiateOrder subclass of ILoggable: " << std::is_base_of_v<ILoggable,NegotiateOrder> << std::endl;

    // std::cout << "Command subclass of Subject: " << std::is_base_of_v<Subject,CommandProcessor> << std::endl;
    // std::cout << "Command subclass of ILoggable: " << std::is_base_of_v<ILoggable,CommandProcessor> << std::endl;

    std::cout << "Order subclass of Subject: " << std::is_base_of_v<Subject,Order> << std::endl;
    std::cout << "Order subclass of ILoggable: " << std::is_base_of_v<ILoggable,Order> << std::endl;

    std::cout << "OrdersList subclass of Subject: " << std::is_base_of_v<Subject,OrdersList> << std::endl;
    std::cout << "OrdersList subclass of ILoggable: " << std::is_base_of_v<ILoggable,OrdersList> << std::endl;

    std::cout << "GameEngine subclass of Subject: " << std::is_base_of_v<Subject,GameEngine> << std::endl;
    std::cout << "GameEngine subclass of ILoggable: " << std::is_base_of_v<ILoggable,GameEngine> << std::endl;

    std::cout << std::noboolalpha;

    ordersList.executeOrders();

    // TODO: Add CommandProcessor tests

    LoadMapCommand loadMapCommand{engine};
    loadMapCommand.attach(logObserver);
    loadMapCommand.saveEffect("Something");

    ValidateMapCommand validateMapCommand{engine};
    validateMapCommand.attach(logObserver);
    validateMapCommand.saveEffect("Something");

    AddPlayerCommand addPlayerCommand{engine};
    addPlayerCommand.attach(logObserver);
    addPlayerCommand.saveEffect("Something");

    AssignTerritoriesCommand assignTerritoriesCommand{engine};
    assignTerritoriesCommand.attach(logObserver);
    assignTerritoriesCommand.saveEffect("Something");

    IssueOrdersCommand issueOrdersCommand{engine};
    issueOrdersCommand.attach(logObserver);
    issueOrdersCommand.saveEffect("Something");

    EndIssueOrdersCommand endIssueOrdersCommand{engine};
    endIssueOrdersCommand.attach(logObserver);
    endIssueOrdersCommand.saveEffect("Something");

    ExecuteOrdersCommand executeOrdersCommand{engine};
    executeOrdersCommand.attach(logObserver);
    executeOrdersCommand.saveEffect("Something");

    EndExecuteOrdersCommand endExecuteOrdersCommand{engine};
    endExecuteOrdersCommand.attach(logObserver);
    endExecuteOrdersCommand.saveEffect("Something");

    WinCommand winCommand{engine};
    winCommand.attach(logObserver);
    winCommand.saveEffect("Something");

    PlayCommand playCommand{engine};
    playCommand.attach(logObserver);
    playCommand.saveEffect("Something");

    QuitCommand quitCommand{engine};
    quitCommand.attach(logObserver);
    quitCommand.saveEffect("Something");

    engine.transition(GameState::mapLoaded);

}