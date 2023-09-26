#include "Map.h"

Map::Map(const std::string &name) {
    territories = new std::map<std::string, Territory>();
    continents = new std::map<std::string, Continent>();
}

Map::Map(const Map &map) {

}

Map::~Map() {
    delete territories;
    delete continents;
}

bool Map::validate() {
    return true;
}

Territory Map::getTerritory(const std::string &territoryName) const {
    return territories->at(territoryName);
}

Continent Map::getContinent(const std::string &continentName) const {
    return continents->at(continentName);
}

std::string Map::getName() const {
    return *name;
}

Territory::Territory(const std::string &name, Continent& continent) {
    this->name = new std::string(name);
    this->armyCount = new unsigned(0);
    owner = nullptr;
    // TODO: add continent
    adjacencies = new std::vector<Territory*>();
    this->continent = &continent;
    continent.addTerritory(*this);
}

Territory::Territory(const Territory &territory) {

}

Territory::~Territory() {
    delete name;
    delete armyCount;
    // only need to delete adjacencies;
    delete adjacencies;

    // release links to other instances
    owner = nullptr;
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

Territory &Territory::addAdjacent(Territory &territory) {
    adjacencies->push_back(&territory);
    return *this;
}

const std::vector<Territory *>& Territory::getAdjacencies() const {
    return *adjacencies;
}

Continent::Continent(const std::string &name, unsigned bonusArmies) {
    this->name = new std::string(name);
    this->bonusArmies = new unsigned(bonusArmies);
    territories = new std::vector<Territory*>();
}

Continent::Continent(const Continent &) {

}

Continent::~Continent() {
    delete name;
    delete bonusArmies;
    // only need to delete adjacencies;
    delete territories;
}

std::string Continent::getName() const {
    return *name;
}

unsigned Continent::getBonusArmies() const {
    return *bonusArmies;
}

Continent &Continent::addTerritory(Territory &territory) {
    territories->push_back(&territory);
    return *this;
}

const std::vector<Territory *> &Continent::getTerritories() const {
    return *territories;
}

Map *MapLoader::load(const std::string &filepath) {
    return nullptr;
}
