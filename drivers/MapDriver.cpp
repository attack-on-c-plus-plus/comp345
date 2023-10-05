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
    }

    for (const auto& dirEntry : std::filesystem::recursive_directory_iterator(mapsPathInvalid)) {
        Map map;
        MapLoader::load(dirEntry.path().string(), map);
    }

//    Map map;
//    MapLoader::load("maps/valid/3D.map", map);

// Example create a map fromm scratch without a file.
//    Continent c(1, "Canada", 7);
//    Continent e(2, "Europe", 5);
//
//    Map map1("name");
//    map1.addContinent(c);
//    map1.addContinent(e);
//
//    Territory o(1, "Ontario", map1.getContinent(1));
//    Territory q(2, "Quebec", map1.getContinent(1));
//    Territory n(3, "New Brunswick", map1.getContinent(1));
//
//    Territory ne(4, "Northern Europe", map1.getContinent(2));
//    Territory se(5, "Southern Europe", map1.getContinent(2));
//    Territory we(6, "Western Europe", map1.getContinent(2));
//
//    map1.addTerritory(o);
//    map1.addTerritory(q);
//    map1.addTerritory(n);
//    map1.addTerritory(ne);
//    map1.addTerritory(se);
//    map1.addTerritory(we);
//
//    map1.addAdjacencies(q, o);
//    map1.addAdjacencies(q, n);
//    map1.addAdjacencies(n, q);
//    map1.addAdjacencies(o, q);
//
//    map1.addAdjacencies(ne, se);
//    map1.addAdjacencies(ne, we);
//    map1.addAdjacencies(se, ne);
//    map1.addAdjacencies(se, we);
//    map1.addAdjacencies(we, ne);
//    map1.addAdjacencies(we, se);
//
//    printContinent(map1, map1.getContinent(1));
//    printContinent(map1, map1.getContinent(2));
//
//    Map map2{map1};
//
//    printContinent(map2, map2.getContinent(1));
//    printContinent(map2, map2.getContinent(2));

}

void printContinent(const Map &m, const Continent &c) {
    std::cout << "Continent " << c.getName() << " has bonus armies: " << c.getBonusArmies() << std::endl;
    for (auto item: m.getAdjacencies(c)) {
        std::cout << "\t" << item.getName() << "\tAdjacencies: " << std::flush;
        for (auto a: m.getAdjacencies(item))
            std::cout << a.getName() << " " << std::flush;
        std::cout << std::endl;
    }
}
