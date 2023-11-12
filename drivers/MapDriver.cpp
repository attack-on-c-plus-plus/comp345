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

#include "MapDriver.h"

#include <filesystem>
#include <iostream>

#include "../Map.h"

void checkCodeCreatedMap();

void testLoadMaps() {
    checkCodeCreatedMap();
    std::cout << "======================" << std::endl;
    std::cout << "Testing map loading..." << std::endl;
    std::cout << "======================" << std::endl;

        // Check naps directory exists
    if (!std::filesystem::exists(mapsPathValid)) {
        std::cout << std::filesystem::path(mapsPathValid) << " directory not found!" << std::endl;
        return;
    }

    if (!std::filesystem::exists(mapsPathInvalid)) {
        std::cout << std::filesystem::path(mapsPathInvalid) << " directory not found!" << std::endl;
        return;
    }

    for (const auto& dirEntry : std::filesystem::recursive_directory_iterator(mapsPathValid)) {
        Map map;
        MapLoader::load(dirEntry.path().string(), map);

        if (map.validate())
            std::cout << "Map is valid!" << std::endl;
        else
            std::cout << "Map is invalid!" << std::endl;
    }

    for (const auto& dirEntry : std::filesystem::recursive_directory_iterator(mapsPathInvalid)) {
        Map map;
        MapLoader::load(dirEntry.path().string(), map);

        if (map.validate())
            std::cout << "Map is valid!" << std::endl;
        else
            std::cout << "Map is invalid!" << std::endl;
    }
}

void checkCodeCreatedMap() {
    Map m{"Test"};
    if (m.validate()) std::cout << "Map is valid!" << std::endl;
    std::cout << std::endl;

    Continent c1{"c1", 1}, c2{"c2", 2};
    m.add(c1).add(c2);
    if (m.validate()) std::cout << "Map is valid!" << std::endl;
    std::cout << std::endl;

    Territory t1{"t1"}, t2{"t2"}, t3{"t3"};
    m.add(t1).add(t2).add(t3);
    if (m.validate()) std::cout << "Map is valid!" << std::endl;
    std::cout << std::endl;

    m.addEdge(t1, t2).addEdge(t1, t3);
    if (m.validate()) std::cout << "Map is valid!" << std::endl;
    std::cout << std::endl;

    m.addEdge(t2, t1);
    if (m.validate()) std::cout << "Map is valid!" << std::endl;
    std::cout << std::endl;

    m.addEdge(t3, t1);
    if (m.validate()) std::cout << "Map is valid!" << std::endl;
    std::cout << std::endl;

    m.addEdge(c1,t1);
    if (m.validate()) std::cout << "Map is valid!" << std::endl;
    std::cout << std::endl;

    m.addEdge(c2,t2).addEdge(c2,t3);
    if (m.validate()) std::cout << "Map is valid!" << std::endl;
}
