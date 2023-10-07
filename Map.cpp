#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <ranges>
#include "Map.h"

// Daniel Soldera
// Carson Senthilkumar
// Joe El-Khoury
// Henri Stephane Carbon
// Haris Mahmood

/**
 * Constructor
 */
Map::Map() : Map("") {
}

/**
 * Constructor
 * @param name
 */
Map::Map(const std::string &name) : name{new std::string(name)}, territories{new std::vector<Territory>},
    continents{new std::vector<Continent>}, adjacencies{new std::vector<std::vector<unsigned>>},
    continentTerritories{new std::vector<std::vector<unsigned>>} {
}

/**
 * Copy constructor
 * @param map
 */
Map::Map(const Map &map) {
    name = new std::string(*map.name);
    continents = new std::vector<Continent>(*map.continents);
    territories = new std::vector<Territory>(*map.territories);
    adjacencies = new std::vector<std::vector<unsigned>>(*map.adjacencies);
    continentTerritories = new std::vector<std::vector<unsigned>>(*map.continentTerritories);
}

/**
 * Destructor
 */
Map::~Map() {
    delete name;
    delete territories;
    delete continents;
    delete adjacencies;
    delete continentTerritories;
}

/**
 * Validates a map
 * @return
 */
bool Map::validate() {
    std::vector<bool> visited;
    visited.resize(territories->size());
    unsigned count = 0;

    // territory graph check
    depthFirstSearchTerritory(visited, count, 0);

    if (count != territories->size())
        return false;

    std::cout << "Map is a connected graph!" << std::endl;

    // Continent graph check
    count = 0;
    for (const auto& ct: *continentTerritories) {
        count += ct.size();
    }

    if (count != territories->size())
        return false;

    std::cout << "Continent are a connected subgraph!" << std::endl;

    // Check each territory has one continent
    for (const auto& t : *territories) {
        if (!t.hasContinent())
            return false;
    }

    std::cout << "Each Territory has a continent!" << std::endl;

    return true;
}

/**
 * Gets a Territory by name
 * @param tName
 * @return
 */
Territory &Map::getTerritory(const std::string &tName) const {
    auto byName = [tName](const Territory& t) { return t.getName() == tName; };
    auto &ts = territories;
    if (auto it = std::find_if(ts->begin(), ts->end(), byName); it != ts->end())
        return *it;
    throw std::out_of_range("Territory with name " + tName + "not found");
}

/**
 * Gets a Continent by name
 * @param cName
 * @return
 */
Continent &Map::getContinent(const std::string &cName) const {
    auto byName = [cName](const Continent& c) { return c.getName() == cName; };
    auto &cs = continents;
    if (auto it = std::find_if(cs->begin(), cs->end(), byName); it != cs->end())
        return *it;
    throw std::out_of_range("Continent with name " + cName + "not found");
}

/**
 * Gets the name of the Map
 * @return
 */
std::string Map::getName() const {
    return *name;
}

/**
 * Gets the count of Territories
 * @return
 */
size_t Map::getTerritoryCount() const {
    return territories->size();
}

/**
 * Gets the count of Continents
 * @return
 */
size_t Map::getContinentCount() const {
    return continents->size();
}

/**
 * Operator= overload
 * @param map
 * @return
 */
Map &Map::operator=(const Map &map) {
    if (this != &map) {
        // clean up resources
        delete name;
        delete continents;
        delete territories;
        delete adjacencies;
        delete continentTerritories;

        name = new std::string(*map.name);
        continents = new std::vector<Continent>(*map.continents);
        territories = new std::vector<Territory>(*map.territories);
        adjacencies = new std::vector<std::vector<unsigned>>(*map.adjacencies);
        continentTerritories = new std::vector<std::vector<unsigned>>(*map.continentTerritories);
    }
    return *this;
}

/**
 * Gets a Continent by id
 * @param id
 * @return the Continent
 */
Continent &Map::getContinent(unsigned id) const {
    return continents->at(id - 1);
}

/**
 * Gets a Territory by id
 * @param id
 * @return the Territory
 */
Territory &Map::getTerritory(unsigned id) const {
    return territories->at(id - 1);
}

/**
 * Gets the Territories belonging to a Continent
 * @param continent
 * @return
 */
std::vector<Territory> Map::getAdjacencies(const Continent &continent) const {
    std::vector<Territory> adj{};
    for (auto i: continentTerritories->at(continent.getId()-1)) {
        adj.push_back(territories->at(i));
    }
    return adj;
}

/**
 * Gets the adjacent Territories to a Territory
 * @param territory
 * @return
 */
std::vector<Territory> Map::getAdjacencies(const Territory &territory) const {
    std::vector<Territory> adj{};
    for (auto i: adjacencies->at(territory.getId()-1)) {
        adj.push_back(territories->at(i));
    }
    return adj;
}

/**
 * Add a Territory to the Map
 * @param t
 * @param continentName
 * @return
 */
Map &Map::addTerritory(Territory &t, const std::string &continentName) {
    *t.id = territories->size() + 1;
    t.continent = &getContinent(continentName);
    continentTerritories->at(t.continent->getId()-1).push_back(t.getId()-1);
    territories->push_back(t);
    adjacencies->emplace_back();
    return *this;
}

/**
 * Add an adjacent Territory to the Territory
 * @param territory
 * @param adjacent
 * @return
 */
Map &Map::addAdjacencies(const Territory& territory, const Territory& adjacent) {
    Territory &t = territories->at(territory.getId() - 1);
    Territory &a = territories->at(adjacent.getId() - 1);
    adjacencies->at(t.getId() - 1).push_back(a.getId() - 1);
    return *this;
}

/**
 * Adds a Continent to the Map
 * @param c
 * @return
 */
Map &Map::addContinent(const Continent& c) {
    *c.id = continents->size() + 1;
    continents->push_back(c);
    continentTerritories->emplace_back();
    return *this;
}

/**
 * Does a depthFirst search to determine is all Territories are connected
 * @param visited
 * @param count
 * @param vertex
 */
void Map::depthFirstSearchTerritory(std::vector<bool> &visited, unsigned &count, unsigned vertex) {
    visited[vertex] = true;
    count++;
    for (auto nextVertex : (*adjacencies)[vertex]) {
        if (!visited[nextVertex]) depthFirstSearchTerritory(visited, count, nextVertex);
    }
}

/**
 * Operator>> overload
 * @param is
 * @param map
 * @return
 */
std::istream &operator>>(std::istream &is, Map &map) {
    std::string token, value, line;
    while(!is.eof() && is.peek() != '[') { // Peek to see if a header is coming.
        MapLoader::readLine(is, line);

        if (line.empty())
            continue;

        MapLoader::parseTokenValuePair(line, token, value);

        if (token != "image")
            continue;

        // image value contains part of the name, so use that.
        std::istringstream n(value);
        std::getline(n, *map.name, '.');
    }
    return is;
}

/**
 * Operator<< overload
 * @param os
 * @param map
 * @return
 */
std::ostream &operator<<(std::ostream &os, const Map &map) {
    os << "Map: " << *map.name << " continents: " << map.continents->size() << " territories: " << map.territories->size();
    return os;
}

/**
 * Gets the Territories belonging to a Player
 * @param p
 * @return
 */
std::vector<Territory> Map::getTerritories(const Player &p) const {
    auto byPlayer = [p](const Territory &territory)
    { return territory.hasOwner() && territory.getOwner().getName() == p.getName(); };
    std::vector<Territory> t{};
    for (const auto& item: ((*territories) | std::views::filter(byPlayer))) {
        t.push_back(item);
    }
    return t;
}

/**
 * Constructor
 */
Territory::Territory() : id{new unsigned(0)}, name{new std::string("")}, armyCount{new unsigned(0)} {
    continent = nullptr;
    owner = nullptr;
}

/**
 * Constructor
 * @param id
 * @param name
 * @param continent
 */
Territory::Territory(unsigned id, const std::string &name, const Continent &continent) {
    this->id = new unsigned(id);
    this->name = new std::string(name);
    this->armyCount = new unsigned(0);
    owner = nullptr;
    this->continent = &continent;
}

/**
 * Copy Constructor
 * @param territory
 */
Territory::Territory(const Territory &territory) {
    id = new unsigned(*territory.id);
    name = new std::string(*territory.name);
    armyCount = new unsigned(*territory.armyCount);
    owner = territory.owner;
    continent = territory.continent;
}

/**
 * Destructor
 */
Territory::~Territory() {
    delete id;
    delete name;
    delete armyCount;

    // release links to other instances
    owner = nullptr;
    continent = nullptr;
}

/**
 * Gets the name of a Territory
 * @return
 */
std::string Territory::getName() const {
    return *name;
}

/**
 * Set the player that owns a Territory
 * @param player
 * @return
 */
Territory &Territory::setOwner(const Player &player) {
    owner = &player;
    return *this;
}

/**
 * Gets the player that owns a Territory
 * @return
 */
Player &Territory::getOwner() const {
    return const_cast<Player &>(*owner);
}

/**
 * Determines if the Territory has an owner
 * @return
 */
bool Territory::hasOwner() const {
    return owner;
}

/**
 * Gets the army count for a Territory
 * @return
 */
unsigned Territory::getArmyCount() const {
    return *armyCount;
}

/**
 * Gets the id of a Territory
 * @return
 */
unsigned Territory::getId() const {
    return *id;
}

/**
 * Operator= overload
 * @param territory
 * @return
 */
Territory &Territory::operator=(const Territory &territory) {
    if (this != &territory) {
        delete id;
        delete name;
        delete armyCount;

        id = new unsigned(*territory.id);
        name = new std::string(*territory.name);
        armyCount = new unsigned(*territory.armyCount);
        owner = territory.owner;
    }
    return *this;
}

/**
 * Determines if a Territory has a Continent
 * @return
 */
bool Territory::hasContinent() const {
    return continent;
}

/**
 * Gets the Continent of a Territory
 * @return
 */
const Continent &Territory::getContinent() const {
    return *continent;
}

/**
 * Operator>> overload
 * @param is
 * @param territory
 * @return
 */
std::istream &operator>>(std::istream &is, Territory &territory) {
    std::string t;
    std::getline(is, *territory.name, ',');
    std::getline(is, t, ',');
    std::getline(is, t, ',');
    return is;
}

/**
 * Operator<< overload
 * @param os
 * @param territory
 * @return
 */
std::ostream &operator<<(std::ostream &os, const Territory &territory) {
    return os;
}

/**
 * Constructor
 */
Continent::Continent() : Continent(0, "", 0) {
}

/**
 * Constructor
 * @param id
 * @param name
 * @param bonusArmies
 */
Continent::Continent(unsigned id, const std::string &name, unsigned bonusArmies) {
    this->id = new unsigned(id);
    this->name = new std::string(name);
    this->bonusArmies = new unsigned(bonusArmies);
}

/**
 * Copy constructor
 * @param continent
 */
Continent::Continent(const Continent &continent) {
    id = new unsigned(*continent.id);
    name = new std::string(*continent.name);
    bonusArmies = new unsigned(*continent.bonusArmies);
}

/**
 * Destructor
 */
Continent::~Continent() {
    delete id;
    delete name;
    delete bonusArmies;
}

/**
 * Get the name of the Continent
 * @return the name
 */
std::string Continent::getName() const {
    return *name;
}

/**
 * Gets the bonus armies for the Continent
 * @return the bonus armies
 */
unsigned Continent::getBonusArmies() const {
    return *bonusArmies;
}

/**
 * Gets the id of the continent
 * @return the id
 */
unsigned Continent::getId() const {
    return *id;
}

/**
 * Operator= overload
 * @param continent
 * @return
 */
Continent &Continent::operator=(const Continent &continent) {
    if (this != &continent) {
        delete id;
        delete name;
        delete bonusArmies;

        id = new unsigned(*continent.id);
        name = new std::string(*continent.name);
        bonusArmies = new unsigned(*continent.bonusArmies);
    }
    return *this;
}

/**
 * Operator>> overload
 * @param is input stream
 * @param continent the continent
 * @return input stream
 */
std::istream &operator>>(std::istream &is, Continent &continent) {
    std::string bonus;
    std::getline(is, *continent.name, '=');
    std::getline(is, bonus);

    if (!bonus.empty()) *continent.bonusArmies = stoi(bonus);

    return is;
}

/**
 * Operator<< overload
 * @param os the out stream
 * @param continent the continent
 * @return the output stream
 */
std::ostream &operator<<(std::ostream &os, const Continent &continent) {
    return os;
}

/**
 * Loads the Map
 * @param filepath the path of the map file
 * @param map the map
 * @return if it was successful
 */
bool MapLoader::load(const std::string &filepath, Map &map) {
    readFile(filepath, map);
    return true;
}


/**
 * Helper method that removes carriage returns from text (needed for linux and macOS)
 * @param s
 */
void MapLoader::removeCarriageReturn(std::string &s) {
    s.erase(std::remove(s.begin(), s.end(), '\r' ), s.end());
}

/**
 * Helper reads the file
 * @param filename the filename to read
 * @param map the map
 */
void MapLoader::readFile(const std::string &filename, Map &map) {
    mapReadState state = MapHeader;
    // Opens the file
    std::ifstream in(filename);
    std::cout << filename << std::flush;
    if (!in.is_open())
        std::cout << " failed to open " << filename << '\n';
    else
    {
        std::string msg;
        // Try to parse the map file
        parseMapFile(map, msg, state, in);
        if (state == Error) { // Map parsing failed does not meet the requirements to be a Conquest Map file
            std::cout << " - " << msg << std::endl;
        }
        else { // Map format is correct but will still need to validate.
            std::cout << " " << map << std::endl;
        }
    }
    in.close();
}

/**
 * Helper parses the map file, eventually will return a complete Map with Continents and Territories loaded.
 * @param map the map
 * @param msg a possible error message
 * @param state the current state
 * @param in an input stream
 */
void MapLoader::parseMapFile(Map &map, std::string &msg, mapReadState &state, std::ifstream &in) {
    while (!in.eof()) {
        switch (state) { // Finite State Machine to go through the Map parsing
            case MapHeader:
                state = readMapHeader(in, msg);
                break;
            case MapSection:
                state = readMapSection(in, msg, map);
                break;
            case ContinentsHeader:
                state = readContinentsHeader(in, msg);
                break;
            case ContinentsSection:
                state = readContinentsSection(in, msg, map);
                break;
            case TerritoriesHeader:
                state = readTerritoriesHeader(in, msg);
                break;
            case TerritoriesSection:
                state = readTerritoriesSection(in, msg, map);
                break;
            case Completed:
            case Error:
                return;
        }
    }
    if (state != Completed && state != Error) { // Hit eof but not Completed or Error, so also an Error
        state = Error;
        msg = "Invalid Map file format - Unexpected end of file";
    }
}

/**
 * Reads the Map Header of the Map file
 * @param in an input stream
 * @param msg a possible error message
 * @returnthe resulting state
 */
mapReadState MapLoader::readMapHeader(std::ifstream &in, std::string &msg) {
    return readHeader(in, msg, MapSection, "[Map]");
}

/**
 * Read the Map Section of the Map File
 * @param in an input stream
 * @param msg a possible error message
 * @param m the map
 * @return the resulting state
 */
mapReadState MapLoader::readMapSection(std::ifstream &in, std::string &msg, Map &m) {
    in >> m;
    if (!m.getName().empty())
        return ContinentsHeader;

    msg = "Invalid Map file format - Invalid [Map] section content.";
    return Error;
}

/**
 * Parses a line into a key and value pair
 * @param line the line to parse
 * @param token the token
 * @param value the value
 */
void MapLoader::parseTokenValuePair(const std::string &line, std::string &token, std::string &value) {
    std::istringstream s(line);
    // Essentially splitting the line on the equals delimiter.
    std::getline(s, token, '=');
    std::getline(s, value);
}

/**
 * Reads the Continents Header of the Map file
 * @param in an input stream
 * @param msg a possible error message
 * @return the resulting state
 */
mapReadState MapLoader::readContinentsHeader(std::ifstream &in, std::string &msg) {
    return readHeader(in, msg, ContinentsSection, "[Continents]");
}

/**
 * Reads the Continents Section of the Map file
 * @param in an input stream
 * @param msg a possible error message
 * @param m the map
 * @return the resulting state
 */
mapReadState MapLoader::readContinentsSection(std::ifstream &in, std::string &msg, Map &m) {
    while(!in.eof() && in.peek() != '[') {
        std::string name, bonusArmies, line;
        readLine(in, line);

        if (line.empty())
            continue;

        Continent c;
        std::istringstream(line) >> c;
        m.addContinent(c);

        if (c.getBonusArmies() == 0 || c.getName().empty()) {
            msg = "Invalid Map file format - Invalid [Continents] section content.";
            return Error;
        }

    }
    return TerritoriesHeader;
}

/**
 * Reads a line from the file
 * @param in an input stream
 * @param line the line read
 */
void MapLoader::readLine(std::istream &in, std::string &line) {
    std::getline(in, line);
    removeCarriageReturn(line);
}

/**
 * Reads the Territory Header of the Map file
 * @param in an input stream
 * @param msg a possible error message
 * @return
 */
mapReadState MapLoader::readTerritoriesHeader(std::ifstream &in, std::string &msg) {
    return readHeader(in, msg, TerritoriesSection, "[Territories]");
}

/**
 * Read the territory section of the map file
 * @param in an input stream
 * @param msg a possible error message
 * @param m the map
 * @return the resulting state
 */
mapReadState MapLoader::readTerritoriesSection(std::ifstream &in, std::string &msg, Map &m) {
    std::vector<std::string> adjacencies;

    while(!in.eof()) {
        if (in.peek() == '[') {
            msg = "Invalid Map file format - Invalid [Territories] section content.";
            return Error;
        }
        std::string continent, line;
        readLine(in, line);

        if (line.empty())
            continue;

        std::istringstream s(line);
        // Essentially splitting the line on the comma delimiter.
        Territory territory;
        s >> territory;

        std::getline(s, continent, ',');

        if (territory.getName().empty() || continent.empty()) {
            msg = "Invalid Map file format - Invalid [Territories] section content.";
            return Error;
        }
        m.addTerritory(territory, continent);

        std::getline(s, line);
        adjacencies.push_back(line);
    }
    if (m.getTerritoryCount() == 0) {
        msg = "Invalid Map file format - No Territories found";
        return Error;
    }

    try {
        // Now that all the territories are added link them.
        std::string tName;
        for (int i = 0; i < adjacencies.size(); ++i) {
            std::istringstream item(adjacencies[i]);
            while (!item.eof()) {
                std::getline(item, tName, ',');
                m.addAdjacencies(m.getTerritory(i + 1), m.getTerritory(tName));
            }
        }
    }
    catch (std::out_of_range &err){
        msg = "Invalid Map File - Adjacent Territory no found";
        return Error;
    }
    return Completed;
}

/**
 * Reads a header in the map file
 * @param in an input stream
 * @param msg the possible error message
 * @param success the next state if successful
 * @param header the header to locate
 * @return the resulting state
 */
mapReadState MapLoader::readHeader(std::ifstream &in, std::string &msg, const mapReadState &success,
                        const std::string &header) {
    std::string line;
    readLine(in, line);
    if (line != header) {
        msg = "Invalid Map file format - " + header + " section not found.";
        return Error;
    }
    return success;
}