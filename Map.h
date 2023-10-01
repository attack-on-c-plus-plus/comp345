#ifndef COMP345_MAP_H
#define COMP345_MAP_H

#include <string>
#include <vector>
#include <map>

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
// temp: need to remove later
class Player{};

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
    [[nodiscard]] bool getTerritory(const std::string &, Territory &) const;
    Map &addTerritory(const Territory&);
    Map &addAdjacencies(const Territory&, const Territory &);
    [[nodiscard]] bool getContinent(const std::string &, Continent &) const;
    Map &addContinent(const Continent&);
    [[nodiscard]] size_t getTerritoryCount() const;
    [[nodiscard]] size_t getContinentCount() const;
    [[nodiscard]] Continent &getContinent(unsigned) const;
    [[nodiscard]] Territory &getTerritory(unsigned) const;
    [[nodiscard]] std::vector<Territory> getAdjacencies(const Continent&) const;
    [[nodiscard]] std::vector<Territory> getAdjacencies(const Territory&) const;
    Map &operator=(const Map&);
private:
    std::string *name;
    std::vector<std::vector<unsigned>> *adjacencies{};
    std::vector<std::vector<unsigned>> *continentTerritories{};
    std::vector<Territory> *territories;
    std::vector<Continent> *continents;
};

std::istream &operator>>(std::istream &is, Map &map);
std::ostream &operator<<(std::istream &is, const Map &map);

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
    [[nodiscard]] const Player &getOwner() const;
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
};

std::istream &operator>>(std::istream &is, Territory &territory);
std::ostream &operator<<(std::istream &is, const Territory &territory);

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
};

std::istream &operator>>(std::istream &is, Continent &continent);
std::ostream &operator<<(std::istream &is, const Continent &continent);

/*
 * MapLoader creates a map from a “Conquest” game map source files. It can read any map from
 * http://www.windowsgames.co.uk/conquest_maps.html. The map loader stores the map as a Map
 * object as described above. The map loader can read any text file (even ones that do not constitute a valid map).
 */
class MapLoader {
public:
    static Map *load(const std::string &filepath);
private:
};

#endif //COMP345_MAP_H
