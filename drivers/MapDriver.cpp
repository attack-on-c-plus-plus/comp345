#include <iostream>
#include <filesystem>
#include "Drivers.h"
#include "../Map.h"

void printContinent(const Map &, const Continent &);

void testLoadMaps() {
    std::cout << "Testing map loading..." << std::endl;

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

        auto t = map.getTerritory(1);
        auto a = map.getAdjacencies(t);

        std::cout << t.getName() << " adjacencies:" << std::endl;
        for (const auto& i: a) {
            std::cout << "\t" << i.getName() << std::endl;
        }

    }

    for (const auto& dirEntry : std::filesystem::recursive_directory_iterator(mapsPathInvalid)) {
        Map map;
        MapLoader::load(dirEntry.path().string(), map);
    }
}