/**
 ************************************
 * COMP 345 Professor Hakim Mellah
 ************************************
 * @author Team 5 Attack on C++
 * @author Daniel Soldera
 * @author Carson Senthilkumar
 * @author Joe El-Khoury
 * @author Henri Stephane Carbon
 * @author Haris Mahmood
 */

#include "Map.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <ranges>
#include <sstream>
#include <string>

#include "Player.h"

/**
 * Constructor
 */
Map::Map() : Map("") {
}

/**
 * Constructor
 * @param name
 */
Map::Map(const std::string &name) : name_{new std::string(name)},
    territories_{new std::vector<Territory*>},
    continents_{new std::vector<const Continent*>},
    territoryEdges_{new std::vector<std::vector<size_t>>},
    continentEdges_{new std::vector<std::vector<size_t>>} {
}

/**
 * Copy constructor
 * @param map
 */
Map::Map(const Map &map) {
    name_ = new std::string(*map.name_);
    continents_ = new std::vector(*map.continents_);
    territories_ = new std::vector(*map.territories_);
    territoryEdges_ = new std::vector(*map.territoryEdges_);
    continentEdges_ = new std::vector(*map.continentEdges_);
}

/**
 * Destructor
 */
Map::~Map() {
    delete name_;
    for(const auto t : *territories_) delete t;
    delete territories_;
    for(const auto c : *continents_) delete c;
    delete continents_;
    delete territoryEdges_;
    delete continentEdges_;
}

/**
 * Validates a map
 * @return
 */
bool Map::validate() {
    if (name_->empty()) {
        std::cout << "Map has no name!" << std::endl;
        return false;
    }
    if (continents_->empty()) {
        std::cout << "Map has no continents!" << std::endl;
        return false;
    }
    if (territories_->empty()) {
        std::cout << "Map has no territories!" << std::endl;
        return false;
    }

    if (!isConnectedGraph() || !hasContinentSubGraphs() || !hasOneContinentPerTerritory())
        return false;

    std::cout << "Map is a connected graph!" << std::endl;
    std::cout << "Continents are a connected subgraph!" << std::endl;
    std::cout << "Each Territory has a continent!" << std::endl;

    return true;
}

/**
 * Gets a Territory by name
 * @param name
 * @return
 */
Territory &Map::territory(const std::string &name) const {
    auto byName = [name](const Territory* t) { return t->name() == name; };
    auto &ts = territories_;
    if (const auto it = std::ranges::find_if(*ts, byName); it != ts->end())
        return **it;
    throw std::out_of_range("Territory with name " + name + " not found");
}

/**
 * Gets a Continent by name
 * @param name
 * @return
 */
const Continent &Map::continent(const std::string &name) const {
    auto byName = [name](const Continent* c) { return c->name() == name; };
    auto &cs = continents_;
    if (const auto it = std::ranges::find_if(*cs, byName); it != cs->end())
        return **it;
    throw std::out_of_range("Continent with name " + name + " not found");
}

/**
 * Gets the name of the Map
 * @return
 */
std::string &Map::name() const {
    return *name_;
}

/**
 * Gets the count of Territories
 * @return
 */
size_t Map::territoryCount() const {
    return territories_->size();
}

/**
 * Gets the count of Continents
 * @return
 */
size_t Map::continentCount() const {
    return continents_->size();
}

/**
 * Operator= overload
 * @param map
 * @return
 */
Map &Map::operator=(const Map &map) {
    if (this != &map) {
        // clean up resources
        delete name_;
        for(const auto t : *territories_) delete t;
        delete territories_;
        for(const auto c : *continents_) delete c;
        delete continents_;
        delete territoryEdges_;
        delete continentEdges_;

        name_ = new std::string(*map.name_);
        continents_ = new std::vector<const Continent*>();
        for (const auto c: *map.continents_) {
            continents_->push_back(new Continent(*c));
        }
        territories_ = new std::vector(*map.territories_);
        for (const auto t: *map.territories_) {
            territories_->push_back(new Territory(*t));
        }
        territoryEdges_ = new std::vector(*map.territoryEdges_);
        continentEdges_ = new std::vector(*map.continentEdges_);
    }
    return *this;
}

/**
 * Gets a Continent by id
 * @param id
 * @return the Continent
 */
const Continent &Map::continent(const size_t id) const {
    return *continents_->at(id);
}

/**
 * Gets a Territory by id
 * @param id
 * @return the Territory
 */
Territory &Map::territory(const size_t id) const {
    return *territories_->at(id);
}

/**
 * Gets the Territories belonging to a Continent
 * @param continent
 * @return
 */
std::vector<const Territory*> Map::adjacencies(const Continent &continent) const {
    std::vector<const Territory*> adj{};
    for (const auto i: continentEdges_->at(continent.id().value())) {
        adj.push_back(territories_->at(i));
    }
    return adj;
}

/**
 * Gets the adjacent Territories to a Territory
 * @param territory
 * @return
 */
std::vector<const Territory*> Map::adjacencies(const Territory &territory) const {
    std::vector<const Territory*> adj{};
    for (const auto i: territoryEdges_->at(territory.id().value())) {
        adj.push_back(territories_->at(i));
    }
    return adj;
}

/**
 * Gets the territories
 */
std::vector<Territory *> &Map::territories() const
{
    return *territories_;
}

/**
 * Add a Territory to the Map
 * @param territory
 * @return
 */
Map &Map::add(const Territory &territory) {
    auto *pt = new Territory(territory);
    *pt->id_ = std::optional(territories_->size());
    territories_->push_back(pt);
    territoryEdges_->emplace_back();
    return *this;
}

/**
 * Add an adjacent Territory to the Continent
 * @param continent
 * @param adjacent
 * @return
 */
Map &Map::addEdge(const Continent& continent, const Territory& adjacent) {
    const auto c = this->continent(continent.name());
    const auto t = this->territory(adjacent.name());
    continentEdges_->at(c.id().value()).push_back(t.id().value());
    return *this;
}

/**
 * Add an adjacent Territory to the Territory
 * @param territory
 * @param adjacent
 * @return
 */
Map &Map::addEdge(const Territory& territory, const Territory& adjacent) {
    const auto t = this->territory(territory.name());
    const auto a = this->territory(adjacent.name());
    territoryEdges_->at(t.id().value()).push_back(a.id().value());
    return *this;
}

/**
 * Adds a Continent to the Map
 * @param c
 * @return
 */
Map &Map::add(const Continent& c) {
    const auto *pc = new Continent(c);
    *pc->id_ = std::optional(continents_->size());
    continents_->push_back(pc);
    continentEdges_->emplace_back();
    return *this;
}

/**
 * Determines if graph is fully connected
 * @return true if fully connected; false otherwise
 */
bool Map::isConnectedGraph() {
    std::vector<bool> visited;
    // try starting from each vertex
    for (int startVertex = 0; startVertex < territories_->size(); ++startVertex) {
        size_t count = 0;
        visited.clear();
        visited.resize(territories_->size());
        // territory graph check
        if (!territoryEdges_->empty())
            depthFirstSearchTerritory(visited, count, startVertex);

        if (count != territories_->size()) { // We weren't able to visit each vertex.
            std::cout << "Map is not a connected graph!" << std::endl;
            return false;
        }
    }
    return true;
}
/**
 * Determines if all continents are sub graphs
 * @return true if all continents are sub graphs; false otherwise
 */
bool Map::hasContinentSubGraphs() const {
    // Continent graph check
    size_t count = 0;
    for (const auto& ct: *continentEdges_) {
        count += ct.size();
    }

    if (count == territories_->size()) return true;

    std::cout << "Continents are not a connected subgraph!" << std::endl;
    return false;
}

/**
 * Checks if all territories have one continent
 * @return true if each territory has one continent; false otherwise
 */
bool Map::hasOneContinentPerTerritory() const {
    std::vector<bool> visited;
    visited.resize(territories_->size());
    for (const auto& ce : *continentEdges_) {
        for (const auto t : ce) {
            if(!visited[t])
                visited[t] = true;
            else {
                std::cout << "Not every Territory has a continent!" << std::endl;
                return false;
            }
        }
    }
    return true;
}

/**
 * Does a depthFirst search to determine is all Territories are connected
 * @param visited
 * @param count
 * @param vertex
 */
void Map::depthFirstSearchTerritory(std::vector<bool> &visited, size_t &count, const unsigned vertex) {
    visited[vertex] = true;
    count++;
    for (const auto nextVertex : (*territoryEdges_)[vertex]) {
        if (!visited[nextVertex]) depthFirstSearchTerritory(visited, count, nextVertex);
    }
}

/**
 * Gets the Territories belonging to a Player
 * @param p
 * @return
 */
std::vector<const Territory*> Map::territories(const Player &p) const {
    auto byPlayer = [p](const Territory *territory)
    { return territory->isOwned() && territory->owner().getName() == p.getName(); };
    std::vector<const Territory*> t{};
    for (const auto& item: *territories_ | std::views::filter(byPlayer)) {
        t.push_back(item);
    }
    return t;
}

const std::vector<const Continent *> &Map::continents() const {
    return *continents_;
}

/**
 * Operator>> overload
 * @param is
 * @param map
 * @return
 */
std::istream &operator>>(std::istream &is, const Map &map) {
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
        std::getline(n, *map.name_, '.');
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
    os << "Map: " << *map.name_ << " continents: " << map.continents_->size() << " territories: " << map.territories_->size();
    return os;
}

/**
 * Constructor
 */
Territory::Territory() : Territory("") {
    owner_ = nullptr;
}

/**
 * Constructor
 * @param name
 */
Territory::Territory(const std::string &name) :
    id_{new std::optional<size_t>(std::nullopt)},
    name_{new std::string(name)},
    armyCount_{new unsigned(0)} {
    owner_ = nullptr;
}

/**
 * Copy Constructor
 * @param territory
 */
Territory::Territory(const Territory &territory) {
    id_ = new std::optional(*territory.id_);
    name_ = new std::string(*territory.name_);
    armyCount_ = new unsigned(*territory.armyCount_);
    owner_ = territory.owner_;
}

/**
 * Destructor
 */
Territory::~Territory() {
    delete id_;
    delete name_;
    delete armyCount_;

    // release links to other instances
    owner_ = nullptr;
}

/**
 * Gets the name of a Territory
 * @return
 */
std::string &Territory::name() const {
    return *name_;
}

/**
 * Set the player that owns a Territory
 * @param player
 * @return
 */
Territory &Territory::owner(const Player &player) {
    owner_ = &player;
    return *this;
}

/**
 * Gets the player that owns a Territory
 * @return
 */
Player &Territory::owner() const {
    return const_cast<Player &>(*owner_);
}

/**
 * Determines if the Territory has an owner
 * @return
 */
bool Territory::isOwned() const {
    return owner_;
}

/**
 * Adds armies to a Territory
 * @return
 */
void Territory::addArmies(const unsigned armies) const {
    *armyCount_ += armies;
}

/**
 * Removes armies from a Territory
 * @return
 */
void Territory::removeArmies(const unsigned armies) const {
    *armyCount_ -= armies;
}

/**
 * Gets the army count for a Territory
 * @return
 */
unsigned Territory::armyCount() const {
    return *armyCount_;
}

/**
 * Gets the id of a Territory
 * @return
 */
std::optional<size_t> Territory::id() const {
    return *id_;
}

/**
 * Operator= overload
 * @param territory
 * @return
 */
Territory &Territory::operator=(const Territory &territory) {
    if (this != &territory) {
        delete id_;
        delete name_;
        delete armyCount_;

        id_ = new std::optional(*territory.id_);
        name_ = new std::string(*territory.name_);
        armyCount_ = new unsigned(*territory.armyCount_);
        owner_ = territory.owner_;
    }
    return *this;
}

/**
 * Operator>> overload
 * @param is
 * @param territory
 * @return
 */
std::istream &operator>>(std::istream &is, const Territory &territory) {
    std::string t;
    std::getline(is, *territory.name_, ',');
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
    os << "Id: " << std::to_string(territory.id_->value()) << " Name: " << *territory.name_;
    return os;
}

/**
 * Constructor
 */
Continent::Continent() : Continent("", 0) {
}

/**
 * Constructor
 * @param name
 * @param bonusArmies
 */
Continent::Continent(const std::string &name, const unsigned bonusArmies) {
    id_ = new std::optional<size_t>();
    name_ = new std::string(name);
    bonusArmies_ = new unsigned(bonusArmies);
}

/**
 * Copy constructor
 * @param continent
 */
Continent::Continent(const Continent &continent) {
    id_ = new std::optional(*continent.id_);
    name_ = new std::string(*continent.name_);
    bonusArmies_ = new unsigned(*continent.bonusArmies_);
}

/**
 * Destructor
 */
Continent::~Continent() {
    delete id_;
    delete name_;
    delete bonusArmies_;
}

/**
 * Get the name of the Continent
 * @return the name
 */
std::string &Continent::name() const {
    return *name_;
}

/**
 * Gets the bonus armies for the Continent
 * @return the bonus armies
 */
unsigned Continent::bonusArmies() const {
    return *bonusArmies_;
}

/**
 * Gets the id of the continent
 * @return the id
 */
std::optional<size_t> Continent::id() const {
    return *id_;
}

/**
 * Operator= overload
 * @param continent
 * @return
 */
Continent &Continent::operator=(const Continent &continent) {
    if (this != &continent) {
        delete id_;
        delete name_;
        delete bonusArmies_;

        id_ = new std::optional(*continent.id_);
        name_ = new std::string(*continent.name_);
        bonusArmies_ = new unsigned(*continent.bonusArmies_);
    }
    return *this;
}

/**
 * Operator>> overload
 * @param is input stream
 * @param continent the continent
 * @return input stream
 */
std::istream &operator>>(std::istream &is, const Continent &continent) {
    std::string bonus;
    std::getline(is, *continent.name_, '=');
    std::getline(is, bonus);

    if (!bonus.empty()) *continent.bonusArmies_ = stoi(bonus);

    return is;
}

/**
 * Operator<< overload
 * @param os the out stream
 * @param continent the continent
 * @return the output stream
 */
std::ostream &operator<<(std::ostream &os, const Continent &continent) {
    os << "Id: " << std::to_string(continent.id_->value()) << " Name: " << *continent.name_;
    return os;
}

/**
 * Loads the Map
 * @param filepath the path of the map file
 * @param map the map
 */
void MapLoader::load(const std::string &filepath, Map &map) {
    readFile(filepath, map);
}


/**
 * Helper method that removes carriage returns from text (needed for linux and macOS)
 * @param s
 */
void MapLoader::removeCarriageReturn(std::string &s) {
    s.erase(std::ranges::remove(s, '\r' ).begin(), s.end());
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
        // Finite State Machine to go through the Map parsing
        switch (state) {
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
                break;
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
mapReadState MapLoader::readMapSection(std::ifstream &in, std::string &msg, const Map &m) {
    in >> m;
    if (!m.name().empty())
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
        m.add(c);

        if (c.bonusArmies() == 0 || c.name().empty()) {
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

        if (territory.name().empty() || continent.empty()) {
            msg = "Invalid Map file format - Invalid [Territories] section content.";
            return Error;
        }

        // find the territory
        m.add(territory);

        m.addEdge(m.continent(continent), territory);

        std::getline(s, line);
        adjacencies.push_back(line);
    }
    if (m.territoryCount() == 0) {
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
                m.addEdge(m.territory(i), m.territory(tName));
            }
        }
    }
    catch ([[maybe_unused]] std::out_of_range &err){
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
