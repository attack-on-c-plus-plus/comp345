#ifndef COMP345_MAP_H
#define COMP345_MAP_H

#include <string>
#include <vector>
#include <map>
#include <optional>

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
    [[nodiscard]] std::string &name() const;
    [[nodiscard]] Territory &territory(const std::string &name) const;
    Map &add(const Territory &t);
    Map &addEdge(const Continent &continent, const Territory &adjacent);
    Map &addEdge(const Territory &territory, const Territory &adjacent);
    [[nodiscard]] Continent &continent(const std::string &name) const;
    Map &add(const Continent &c);
    [[nodiscard]] size_t territoryCount() const;
    [[nodiscard]] size_t continentCount() const;
    [[nodiscard]] Continent &continent(size_t id) const;
    [[nodiscard]] Territory &territory(size_t id) const;
    [[nodiscard]] std::vector<const Territory*> territories(const Player &p) const;
    [[nodiscard]] std::vector<Territory *> &territories() const;
    [[nodiscard]] std::vector<const Territory*> adjacencies(const Continent &continent) const;
    [[nodiscard]] std::vector<const Territory*> adjacencies(const Territory &territory) const;
    Map &operator=(const Map&);
private:
    std::string *name_;
    std::vector<std::vector<size_t>> *territoryEdges_{};
    std::vector<std::vector<size_t>> *continentEdges_{};
    std::vector<Territory*> *territories_;
    std::vector<Continent*> *continents_;
    bool isConnectedGraph();
    bool hasContinentSubGraphs();
    bool hasOneContinentPerTerritory();
    void depthFirstSearchTerritory(std::vector<bool> &visited, size_t &count, unsigned vertex);
    friend std::istream &operator>>(std::istream &is, Map &map);
    friend std::ostream &operator<<(std::ostream &os, const Map &map);
};

/*
 * Territory can have any number of adjacent territories.
 */
class Territory {
public:
    Territory();
    explicit Territory(const std::string &);
    // Copy constructor
    Territory(const Territory&);
    // Destructor
    ~Territory();
    [[nodiscard]] std::optional<size_t> id() const;
    [[nodiscard]] std::string &name() const;
    Territory &owner(const Player &player);
    [[nodiscard]] Player &owner() const;
    [[nodiscard]] bool isOwned() const;
    [[nodiscard]] unsigned armyCount() const;
    Territory &operator=(const Territory&);
    unsigned addArmies(int armies);
    unsigned removeArmies(int armies);
private:
    std::optional<size_t> *id_;
    std::string *name_;
    const Player *owner_;
    unsigned *armyCount_;
    friend std::istream &operator>>(std::istream &is, Territory &territory);
    friend std::ostream &operator<<(std::ostream &os, const Territory &territory);
    friend Map &Map::add(const Territory &t);
};

/*
 * Continents are connected subgraphs, where each territory belongs to one and only one continent. Each territory is owned by a player and contain a number of armies. The Map class can be used to represent any map configuration.
*/
class Continent {
public:
    Continent();
    explicit Continent(const std::string &, unsigned);
    // Copy constructor
    Continent(const Continent&);
    // Destructor
    ~Continent();
    [[nodiscard]] std::optional<size_t> id() const;
    [[nodiscard]] std::string &name() const;
    [[nodiscard]] unsigned bonusArmies() const;
    Continent &operator=(const Continent &);
private:
    std::optional<size_t> *id_;
    std::string *name_;
    unsigned *bonusArmies_;
    friend std::istream &operator>>(std::istream &is, Continent &continent);
    friend std::ostream &operator<<(std::ostream &os, const Continent &continent);
    friend Map &Map::add(const Continent& c);
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
    static void load(const std::string &, Map &);
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
