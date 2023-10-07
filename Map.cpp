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

Map::Map() : Map("") {
}

Map::Map(const std::string &name) : name{new std::string(name)}, territories{new std::vector<Territory>},
    continents{new std::vector<Continent>}, adjacencies{new std::vector<std::vector<unsigned>>},
    continentTerritories{new std::vector<std::vector<unsigned>>} {
}

Map::Map(const Map &map) {
    name = new std::string(*map.name);
    continents = new std::vector<Continent>(*map.continents);
    territories = new std::vector<Territory>(*map.territories);
    adjacencies = new std::vector<std::vector<unsigned>>(*map.adjacencies);
    continentTerritories = new std::vector<std::vector<unsigned>>(*map.continentTerritories);
}

Map::~Map() {
    delete name;
    delete territories;
    delete continents;
    delete adjacencies;
    delete continentTerritories;
}

bool Map::validate() {
    bool *visited = new bool[territories->size()];
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

Territory &Map::getTerritory(const std::string &tName) const {
    auto byName = [tName](const Territory& t) { return t.getName() == tName; };
    auto &ts = territories;
    if (auto it = std::find_if(ts->begin(), ts->end(), byName); it != ts->end())
        return *it;
    throw std::out_of_range("Territory with name " + tName + "not found");
}

Continent &Map::getContinent(const std::string &cName) const {
    auto byName = [cName](const Continent& c) { return c.getName() == cName; };
    auto &cs = continents;
    if (auto it = std::find_if(cs->begin(), cs->end(), byName); it != cs->end())
        return *it;
    throw std::out_of_range("Continent with name " + cName + "not found");
}

std::string Map::getName() const {
    return *name;
}

size_t Map::getTerritoryCount() const {
    return territories->size();
}

size_t Map::getContinentCount() const {
    return continents->size();
}

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

Continent &Map::getContinent(unsigned id) const {
    return continents->at(id - 1);
}map validate

Territory &Map::getTerritory(unsigned id) const {
    return territories->at(id - 1);
}

std::vector<Territory> Map::getAdjacencies(const Continent &continent) const {
    std::vector<Territory> adj{};
    for (auto i: continentTerritories->at(continent.getId()-1)) {
        adj.push_back(territories->at(i));
    }
    return adj;
}

std::vector<Territory> Map::getAdjacencies(const Territory &territory) const {
    std::vector<Territory> adj{};
    for (auto i: adjacencies->at(territory.getId()-1)) {
        adj.push_back(territories->at(i));
    }
    return adj;
}

Map &Map::addTerritory(Territory &t, const std::string &continentName) {
    *t.id = territories->size() + 1;
    t.continent = &getContinent(continentName);
    continentTerritories->at(t.continent->getId()-1).push_back(t.getId()-1);
    territories->push_back(t);
    adjacencies->emplace_back();
    return *this;
}

Map &Map::addAdjacencies(const Territory& territory, const Territory& adjacent) {
    Territory &t = territories->at(territory.getId() - 1);
    Territory &a = territories->at(adjacent.getId() - 1);
    adjacencies->at(t.getId() - 1).push_back(a.getId() - 1);
    return *this;
}

Map &Map::addContinent(const Continent& c) {
    *c.id = continents->size() + 1;
    continents->push_back(c);
    continentTerritories->emplace_back();
    return *this;
}

void Map::depthFirstSearchTerritory(bool visited[], unsigned &count, unsigned vertex) {
    visited[vertex] = true;
    count++;
    for (auto nextVertex : (*adjacencies)[vertex]) {
        if (!visited[nextVertex]) depthFirstSearchTerritory(visited, count, nextVertex);
    }
}

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

std::ostream &operator<<(std::ostream &os, const Map &map) {
    os << "Map: " << *map.name << " continents: " << map.continents->size() << " territories: " << map.territories->size();
    return os;
}

std::vector<Territory> Map::getTerritories(const Player &p) const {
    auto byPlayer = [p](const Territory& territory) { return territory.getOwner().getName() == p.getName(); };
    std::vector<Territory> t{};
    for (const auto& item: ((*territories) | std::views::filter(byPlayer))) {
        t.push_back(item);
    }
    return t;
}

Territory::Territory() : id{new unsigned(0)}, name{new std::string("")}, armyCount{new unsigned(0)} {
    continent = nullptr;
    owner = nullptr;
}

Territory::Territory(unsigned id, const std::string &name, const Continent &continent) {
    this->id = new unsigned(id);
    this->name = new std::string(name);
    this->armyCount = new unsigned(0);
    owner = nullptr;
    this->continent = &continent;
}

Territory::Territory(const Territory &territory) {
    id = new unsigned(*territory.id);
    name = new std::string(*territory.name);
    armyCount = new unsigned(*territory.armyCount);
    owner = territory.owner;
    continent = territory.continent;
}

Territory::~Territory() {
    delete id;
    delete name;
    delete armyCount;

    // release links to other instances
    owner = nullptr;
    continent = nullptr;
}

std::string Territory::getName() const {
    return *name;
}

Territory &Territory::setOwner(const Player &player) {
    owner = &player;
    return *this;
}

Player &Territory::getOwner() const {
    return const_cast<Player &>(*owner);
}

bool Territory::hasOwner() const {
    return owner;
}

unsigned Territory::getArmyCount() const {
    return *armyCount;
}

unsigned Territory::getId() const {
    return *id;
}

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

bool Territory::hasContinent() const {
    return continent;
}

const Continent &Territory::getContinent() const {
    return *continent;
}

std::istream &operator>>(std::istream &is, Territory &territory) {
    std::string t;
    std::getline(is, *territory.name, ',');
    std::getline(is, t, ',');
    std::getline(is, t, ',');
    return is;
}

std::ostream &operator<<(std::ostream &os, const Territory &territory) {
    return os;
}

// Continent

Continent::Continent() : Continent(0, "", 0) {
}

Continent::Continent(unsigned id, const std::string &name, unsigned bonusArmies) {
    this->id = new unsigned(id);
    this->name = new std::string(name);
    this->bonusArmies = new unsigned(bonusArmies);
}

Continent::Continent(const Continent &continent) {
    id = new unsigned(*continent.id);
    name = new std::string(*continent.name);
    bonusArmies = new unsigned(*continent.bonusArmies);
}

Continent::~Continent() {
    delete id;
    delete name;
    delete bonusArmies;
}

std::string Continent::getName() const {
    return *name;
}

unsigned Continent::getBonusArmies() const {
    return *bonusArmies;
}

unsigned Continent::getId() const {
    return *id;
}

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

std::istream &operator>>(std::istream &is, Continent &continent) {
    std::string bonus;
    std::getline(is, *continent.name, '=');
    std::getline(is, bonus);

    if (!bonus.empty()) *continent.bonusArmies = stoi(bonus);

    return is;
}

std::ostream &operator<<(std::ostream &os, const Continent &continent) {
    return os;
}

bool MapLoader::load(const std::string &filepath, Map &map) {
    readFile(filepath, map);
    return true;
}

// Helper method that removes carriage returns from text (needed for linux and macOS)
void MapLoader::removeCarriageReturn(std::string &s) {
    s.erase(std::remove(s.begin(), s.end(), '\r' ), s.end());
}

// Helper reads the file
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

// Helper parses the map file, eventually will return a complete Map with Continents and Territories loaded.
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

mapReadState MapLoader::readMapHeader(std::ifstream &in, std::string &msg) {
    return readHeader(in, msg, MapSection, "[Map]");
}

mapReadState MapLoader::readMapSection(std::ifstream &in, std::string &msg, Map &m) {
    in >> m;
    if (!m.getName().empty())
        return ContinentsHeader;

    msg = "Invalid Map file format - Invalid [Map] section content.";
    return Error;
}

void MapLoader::parseTokenValuePair(const std::string &line, std::string &token, std::string &value) {
    std::istringstream s(line);
    // Essentially splitting the line on the equals delimiter.
    std::getline(s, token, '=');
    std::getline(s, value);
}

mapReadState MapLoader::readContinentsHeader(std::ifstream &in, std::string &msg) {
    return readHeader(in, msg, ContinentsSection, "[Continents]");
}

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

void MapLoader::readLine(std::istream &in, std::string &line) {
    std::getline(in, line);
    removeCarriageReturn(line);
}

mapReadState MapLoader::readTerritoriesHeader(std::ifstream &in, std::string &msg) {
    return readHeader(in, msg, TerritoriesSection, "[Territories]");
}

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