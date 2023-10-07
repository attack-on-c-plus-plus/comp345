#include <iostream>
#include <filesystem>
#include "Drivers.h"
#include "../Map.h"

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

        if (map.validate())
            std::cout << "Map is valid!" << std::endl;
        else
            std::cout << "Map is invalid!" << std::endl;
    }

    for (const auto& dirEntry : std::filesystem::recursive_directory_iterator(mapsPathInvalid)) {
        Map map;
        MapLoader::load(dirEntry.path().string(), map);
    }
}