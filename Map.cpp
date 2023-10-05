#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <ranges>
#include "Map.h"

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

void removeCarriageReturn(std::string &);
static void readFile(const std::string &);
mapReadState readMapHeader(std::ifstream &in, std::string &msg);
mapReadState readMapSection(std::ifstream &, std::string &, Map &);
mapReadState readContinentsHeader(std::ifstream &, std::string &);
mapReadState readContinentsSection(std::ifstream &, std::string &, Map &);
mapReadState readTerritoriesHeader(std::ifstream &, std::string &);
mapReadState readTerritoriesSection(std::ifstream &, std::string &, Map &);
mapReadState readHeader(std::ifstream &in, std::string &msg, const mapReadState &success,
                const std::string &header);
static void parseMapFile(Map &map, std::string &msg, mapReadState &state, std::ifstream &in);

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
    return true;
}

bool Map::getTerritory(const std::string &territoryName, Territory &territory) const {
    for (const auto& t: *territories) {
        if (t.getName() == territoryName) {
            territory = t;
            return true;
        }
    }
    return false;
}

bool Map::getContinent(const std::string &continentName, Continent &continent) const {
    for (const auto& c: *continents) {
        if (c.getName() == continentName) {
            continent = c;
            return true;
        }
    }
    return false;
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
}

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

Map &Map::addTerritory(const Territory &t) {
    territories->push_back(t);
    adjacencies->emplace_back();
    continentTerritories->at(t.getContinent().getId()-1).push_back(t.getId()-1);
    return *this;
}

Map &Map::addAdjacencies(const Territory& territory, const Territory& adjacent) {
    Territory &t = territories->at(territory.getId() - 1);
    Territory &a = territories->at(adjacent.getId() - 1);
    adjacencies->at(t.getId() - 1).push_back(a.getId() - 1);
    return *this;
}

Map &Map::addContinent(const Continent& c) {
    continents->push_back(c);
    continentTerritories->emplace_back();
    return *this;
}

std::vector<Territory> Map::getTerritories(const Player &p) const {
    auto byPlayer = [p](const Territory& territory) { return territory.getOwner() == p; };
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

const Player& Territory::getOwner() const {
    return *owner;
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

bool MapLoader::load(const std::string &filepath, Map &map) {
    readFile(filepath);
    return true;
}

// Helper method that removes carriage returns from text (needed for linux and macOS)
void removeCarriageReturn(std::string &s) {
    s.erase(std::remove(s.begin(), s.end(), '\r' ), s.end());
}

// Helper reads the file
static void readFile(const std::string &filename) {
    mapReadState state = MapHeader;
    // Opens the file
    std::ifstream in(filename);
    std::cout << filename << std::flush;
    if (!in.is_open())
        std::cout << "failed to open " << filename << '\n';
    else
    {
        Map map;
        std::string msg;
        // Try to parse the map file
        parseMapFile(map, msg, state, in);
        if (state == Error) { // Map parsing failed does not meet the requirements to be a Conquest Map file
            std::cout << " - " << msg << std::endl;
        }
        else { // Map format is correct but will still need to validate.
            std::cout << " loaded" << std::endl;
        }
    }
    in.close();
}

// Helper parses the map file, eventually will return a complete Map with Continents and Territories loaded.
static void parseMapFile(Map &map, std::string &msg, mapReadState &state, std::ifstream &in) {
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

mapReadState readMapHeader(std::ifstream &in, std::string &msg) {
    return readHeader(in, msg, MapSection, "[Map]");
}

mapReadState readMapSection(std::ifstream &in, std::string &msg, Map &m) {
    bool isNameFound = false;
    while(!in.eof() && in.peek() != '[') { // Peek to see if a header is coming.
        std::string token, value, line, name;
        std::getline(in, line);
        removeCarriageReturn(line);

        if (!line.empty()) {
            std::istringstream s(line);
            // Essentially splitting the line on the equals delimiter.
            std::getline(s, token, '=');
            std::getline(s, value);
            if (token == "image") { // image value contains part of the name, so use that.
                std::istringstream n(value);
                std::getline(n, name, '.');
                m = Map(name); // Have a name create a Map instance.
                isNameFound = true;
            }
        }
    }
    if (!isNameFound) {
        msg = "Invalid Map file format - Invalid [Map] section content.";
        return Error;
    }
    return ContinentsHeader;
}

mapReadState readContinentsHeader(std::ifstream &in, std::string &msg) {
    return readHeader(in, msg, ContinentsSection, "[Continents]");
}

mapReadState readContinentsSection(std::ifstream &in, std::string &msg, Map &m) {
    while(!in.eof() && in.peek() != '[') {
        std::string name, bonusArmies, line;
        std::getline(in, line);
        removeCarriageReturn(line);

        if (!line.empty()) {
            std::istringstream s(line);
            // Essentially splitting the line on the equals delimiter.
            std::getline(s, name, '=');
            std::getline(s, bonusArmies);

            if (bonusArmies.empty() || name.empty()) {
                msg = "Invalid Map file format - Invalid [Continents] section content.";
                return Error;
            }
        }
    }
    return TerritoriesHeader;
}
mapReadState readTerritoriesHeader(std::ifstream &in, std::string &msg) {
    return readHeader(in, msg, TerritoriesSection, "[Territories]");
}

mapReadState readTerritoriesSection(std::ifstream &in, std::string &msg, Map &m) {
    unsigned territoryCount = 0;
    while(!in.eof()) {
        if (in.peek() == '[') {
            msg = "Invalid Map file format - Invalid [Territories] section content.";
            return Error;
        }
        std::string name, x, y, continent, adjacencies, line;
        std::getline(in, line);
        removeCarriageReturn(line);

        if (!line.empty()) {
            std::istringstream s(line);
            // Essentially splitting the line on the comma delimiter.
            std::getline(s, name, ',');
            std::getline(s, x, ',');
            std::getline(s, y, ',');
            std::getline(s, continent, ',');
            std::getline(s, adjacencies);

            if (name.empty() || x.empty() || y.empty() || continent.empty() || adjacencies.empty()) {
                msg = "Invalid Map file format - Invalid [Territories] section content.";
                return Error;
            }
            territoryCount++;
        }
    }
    if (territoryCount == 0) {
        msg = "Invalid Map file format - No Territories found";
        return Error;
    }
    return Completed;
}

mapReadState readHeader(std::ifstream &in, std::string &msg, const mapReadState &success,
                        const std::string &header) {
    std::string line;
    std::getline(in, line);
    removeCarriageReturn(line);
    if (line != header) {
        msg = "Invalid Map file format - " + header + " section not found.";
        return Error;
    }
    return success;
}
