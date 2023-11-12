cmake_minimum_required(VERSION 3.26)
project(comp345)

set(CMAKE_CXX_STANDARD 20)

add_executable(comp345 MainDriver.cpp
        Map.cpp
        Map.h
        Player.cpp
        Player.h
        Orders.cpp
        Orders.h
        Cards.cpp
        Cards.h
        GameEngine.cpp
        GameEngine.h
        LoggingObserver.cpp
        LoggingObserver.h
        CommandProcessing.cpp
        CommandProcessing.h
        drivers/MapDriver.cpp
        drivers/GameEngineDriver.cpp
        drivers/CardsDriver.cpp
        drivers/OrdersDriver.cpp
        drivers/PlayerDriver.cpp
        drivers/LoggingObserverDriver.cpp
        drivers/CommandProcessingDriver.cpp
        drivers/CommandProcessingDriver.h
        drivers/CardsDriver.h
        drivers/GameEngineDriver.h
        drivers/LoggingObserverDriver.h
        drivers/MapDriver.h
        drivers/OrdersDriver.h
        drivers/PlayerDriver.h)
file(REMOVE_RECURSE "${CMAKE_CURRENT_BINARY_DIR}/maps")
file(COPY maps/invalid DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/maps")
file(COPY maps/valid DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/maps")
file(REMOVE_RECURSE "${CMAKE_CURRENT_BINARY_DIR}/res")
file(COPY res DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
