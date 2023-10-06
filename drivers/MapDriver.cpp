#include <iostream>
#include <filesystem>
#include "../Map.h"

void printContinent(const Map &, const Continent &);

void testLoadMaps() {
    std::cout << "Testing map loading..." << std::endl;

    for (const auto& dirEntry : std::filesystem::recursive_directory_iterator("maps/valid")) {
        Map map;
        MapLoader::load(dirEntry.path().string(), map);

        auto t = map.getTerritory(1);
        auto a = map.getAdjacencies(t);

        std::cout << t.getName() << " adjacencies:" << std::endl;
        for (const auto& i: a) {
            std::cout << "\t" << i.getName() << std::endl;
        }

    }

    for (const auto& dirEntry : std::filesystem::recursive_directory_iterator("maps/invalid")) {
        Map map;
        MapLoader::load(dirEntry.path().string(), map);
    }
}