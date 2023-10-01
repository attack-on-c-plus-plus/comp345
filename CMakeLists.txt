cmake_minimum_required(VERSION 3.26)
project(comp345)

set(CMAKE_CXX_STANDARD 20)

add_executable(comp345 main.cpp
        Map.cpp
        Map.h
        GameEngine.cpp
        GameEngine.h
        drivers/MapDriver.cpp
        drivers/Drivers.h
        drivers/GameEngineDriver.cpp
        Cards.cpp
        drivers/CardsDriver.cpp)