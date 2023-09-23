cmake_minimum_required(VERSION 3.26)
project(comp345)

set(CMAKE_CXX_STANDARD 20)

add_executable(comp345 main.cpp
        drivers/MapDriver.cpp
        drivers/Drivers.h)