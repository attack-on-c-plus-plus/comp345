#ifndef COMP345_MAP_H
#define COMP345_MAP_H

#include <string>

/*
 * Map.h / Map.cpp incorporates a group of classes that implement the structure and operation of a map for the game
 * Warzone (https://www.warzone.com/).
 *
 */

// Basic definition to avoid definition order within file.
class Map;
class MapLoader;
class Territory;
class Continent;

/*
 * Map is implemented as a connected graph, where each node represents a territory. Edges between nodes represent adjacency between territories.
*/
class Map {
public:
    explicit Map(const std::string &name);
    // Copy constructor
    Map(const Map&);
    // Destructor
    ~Map();
    bool validate();
private:
    std::string* name;
};

/*
 * Territory can have any number of adjacent territories.
 */
class Territory {
public:
    explicit Territory(const std::string &name);
    // Copy constructor
    Territory(const Territory&);
    // Destructor
    ~Territory();
private:
    std::string* name;
    // Player* owner;
};

/*
 * Continents are connected subgraphs, where each territory belongs to one and only one continent. Each territory is owned by a player and contain a number of armies. The Map class can be used to represent any map configuration.
*/
class Continent {
public:
    explicit Continent(const std::string &name);
    // Copy constructor
    Continent(const Continent&);
    // Destructor
    ~Continent();
private:
    std::string* name;
};

/*
 * MapLoader creates a map from a “Conquest” game map source files. It can read any map from
 * http://www.windowsgames.co.uk/conquest_maps.html. The map loader stores the map as a Map
 * object as described above. The map loader can read any text file (even ones that do not constitute a valid map).
 */
class MapLoader {
public:
    static Map* load(const std::string* filepath);
private:
};

#endif //COMP345_MAP_H
