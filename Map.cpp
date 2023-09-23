#include "Map.h"

Map::Map(const std::string &name) {
    territories = new std::map<std::string, Territory>();
    continents = new std::map<std::string, Continent>();
}

Map::Map(const Map &) {

}

Map::~Map() {
    delete territories;
    delete continents;
}

bool Map::validate() {
    return true;
}

Territory Map::getTerritory(const std::string &name) const {
    return territories->at(name);
}

Continent Map::getContinent(const std::string &name) const {
    return continents->at(name);
}

std::string Map::getName() const {
    return *name;
}

Territory::Territory(const std::string &name) {

}

Territory::Territory(const Territory &) {

}

Territory::~Territory() {

}

std::string Territory::getName() const {
    return *name;
}

Territory &Territory::setOwner(const Player &player) {
    return *this;
}

Player Territory::getOwner() const {
    return *owner;
}

Continent::Continent(const std::string &name, unsigned bonusArmies) {

}

Continent::Continent(const Continent &) {

}

Continent::~Continent() {

}

std::string Continent::getName() const {
    return *name;
}

unsigned Continent::getBonusArmies() const {
    return *bonusArmies;
}

Map *MapLoader::load(const std::string &filepath) {
    return nullptr;
}
