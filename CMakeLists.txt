cmake_minimum_required(VERSION 3.26)
project(comp345)

set(CMAKE_CXX_STANDARD 20)

add_executable(comp345 main.cpp
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
        drivers/MapDriver.cpp
        drivers/Drivers.h
        drivers/GameEngineDriver.cpp
        drivers/CardsDriver.cpp
        drivers/OrdersDriver.cpp
        drivers/PlayerDriver.cpp)
file(REMOVE_RECURSE "${CMAKE_CURRENT_BINARY_DIR}/maps")
file(COPY maps/invalid DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/maps")
file(COPY maps/valid DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/maps")
