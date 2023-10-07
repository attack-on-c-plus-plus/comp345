#ifndef COMP345_MAP_H
#define COMP345_MAP_H

#include <string>
#include <vector>
#include <map>
#include "Player.h"

// Daniel Soldera
// Carson Senthilkumar
// Joe El-Khoury
// Henri Stephane Carbon
// Haris Mahmood

/*
 * Map.h / Map.cpp incorporates a group of classes that implement the structure and operation of a map for the game
 * Warzone (https://www.warzone.com/).
 *
 */

// Basic definition to avoid definition order within file.
class Player;
class Map;
class MapLoader;
class Territory;
class Continent;

/*
 * Map is implemented as a connected graph, where each node represents a territory. Edges between nodes represent adjacency between territories.
*/
class Map {
public:
    Map();
    explicit Map(const std::string &);
    // Copy constructor
    Map(const Map&);
    // Destructor
    ~Map();
    bool validate();
    [[nodiscard]] std::string getName() const;
    [[nodiscard]] Territory &getTerritory(const std::string &) const;
    Map &addTerritory(Territory &t, const std::string &continentName);
    Map &addAdjacencies(const Territory&, const Territory &);
    [[nodiscard]] Continent &getContinent(const std::string &) const;
    Map &addContinent(const Continent&);
    [[nodiscard]] size_t getTerritoryCount() const;
    [[nodiscard]] size_t getContinentCount() const;
    [[nodiscard]] Continent &getContinent(unsigned) const;
    [[nodiscard]] Territory &getTerritory(unsigned) const;
    [[nodiscard]] std::vector<Territory> getTerritories(const Player&) const;
    [[nodiscard]] std::vector<Territory> getAdjacencies(const Continent&) const;
    [[nodiscard]] std::vector<Territory> getAdjacencies(const Territory&) const;
    Map &operator=(const Map&);
private:
    std::string *name;
    std::vector<std::vector<unsigned>> *adjacencies{};
    std::vector<std::vector<unsigned>> *continentTerritories{};
    std::vector<Territory> *territories;
    std::vector<Continent> *continents;
    void depthFirstSearchTerritory(std::vector<bool> &visited, unsigned &count, unsigned vertex);
    friend std::istream &operator>>(std::istream &is, Map &map);
    friend std::ostream &operator<<(std::ostream &os, const Map &map);
};

/*
 * Territory can have any number of adjacent territories.
 */
class Territory {
public:
    Territory();
    explicit Territory(unsigned, const std::string &, const Continent &);
    // Copy constructor
    Territory(const Territory&);
    // Destructor
    ~Territory();
    [[nodiscard]] unsigned getId() const;
    [[nodiscard]] std::string getName() const;
    Territory &setOwner(const Player &player);
    [[nodiscard]] Player &getOwner() const;
    [[nodiscard]] bool hasOwner() const;
    [[nodiscard]] unsigned getArmyCount() const;
    [[nodiscard]] const Continent &getContinent() const;
    [[nodiscard]] bool hasContinent() const;
    Territory &operator=(const Territory&);
private:
    unsigned *id;
    std::string *name;
    const Player *owner;
    unsigned *armyCount;
    const Continent *continent;
    friend std::istream &operator>>(std::istream &is, Territory &territory);
    friend std::ostream &operator<<(std::ostream &os, const Territory &territory);
    friend Map &Map::addTerritory(Territory &t, const std::string &continentName);
};

/*
 * Continents are connected subgraphs, where each territory belongs to one and only one continent. Each territory is owned by a player and contain a number of armies. The Map class can be used to represent any map configuration.
*/
class Continent {
public:
    Continent();
    explicit Continent(unsigned, const std::string &, unsigned);
    // Copy constructor
    Continent(const Continent&);
    // Destructor
    ~Continent();
    [[nodiscard]] std::string getName() const;
    [[nodiscard]] unsigned getBonusArmies() const;
    Continent &operator=(const Continent &);
    [[nodiscard]] unsigned getId() const;
private:
    unsigned *id;
    std::string *name;
    unsigned *bonusArmies;
    friend std::istream &operator>>(std::istream &is, Continent &continent);
    friend std::ostream &operator<<(std::ostream &os, const Continent &continent);
    friend Map &Map::addContinent(const Continent& c);
};

enum mapReadState {
    MapHeader,
    MapSection,
    ContinentsHeader,
    ContinentsSection,
    TerritoriesHeader,
    TerritoriesSection,
    Completed,
    Error
};

/*
 * MapLoader creates a map from a “Conquest” game map source files. It can read any map from
 * http://www.windowsgames.co.uk/conquest_maps.html. The map loader stores the map as a Map
 * object as described above. The map loader can read any text file (even ones that do not constitute a valid map).
 */
class MapLoader {
public:
    static bool load(const std::string &, Map &);
    static void readLine(std::istream &in, std::string &line);
    static void parseTokenValuePair(const std::string &line, std::string &token, std::string &value);
private:
    static void removeCarriageReturn(std::string &);
    static void readFile(const std::string &, Map &);
    static mapReadState readMapHeader(std::ifstream &in, std::string &msg);
    static mapReadState readMapSection(std::ifstream &, std::string &, Map &);
    static mapReadState readContinentsHeader(std::ifstream &, std::string &);
    static mapReadState readContinentsSection(std::ifstream &, std::string &, Map &);
    static mapReadState readTerritoriesHeader(std::ifstream &, std::string &);
    static mapReadState readTerritoriesSection(std::ifstream &, std::string &, Map &);
    static mapReadState readHeader(std::ifstream &in, std::string &msg, const mapReadState &success,
                            const std::string &header);
    static void parseMapFile(Map &map, std::string &msg, mapReadState &state, std::ifstream &in);


};



#endif //COMP345_MAP_H
