#include "Map.h"

Map::Map(const std::string &name) {
}

Map::Map(const Map &) {

}

Map::~Map() {

}

bool Map::validate() {
    return true;
}

Territory::Territory(const std::string &name) {

}

Territory::Territory(const Territory &) {

}

Territory::~Territory() {

}

Continent::Continent(const std::string &name) {

}

Continent::Continent(const Continent &) {

}

Continent::~Continent() {

}

Map *MapLoader::load(const std::string *const filepath) {
    return nullptr;
}
