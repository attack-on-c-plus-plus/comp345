#include "Map.h"

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

Map *MapLoader::load(const std::string &filepath) {
    return nullptr;
}
