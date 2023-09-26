#include <iostream>
#include "../Map.h"

void testLoadMaps() {
    std::cout << "Testing map loading..." << std::endl;

    auto *c = new Continent("Canada", 7);
    auto *e = new Continent("Europe", 5);

    auto *o = new Territory("Ontario", *c);
    auto *q = new Territory("Quebec", *c);
    auto *n = new Territory("New Brunswick", *c);

    auto *ne = new Territory("Northern Europe", *e);
    auto *se = new Territory("Southern Europe", *e);
    auto *we = new Territory("Western Europe", *e);

    auto *p1 = new Player();
    auto *p2 = new Player();

    std::cout << q->getName() << std::endl;
    std::cout << o->getName() << std::endl;
    std::cout << n->getName() << std::endl;

    std::cout << ne->getName() << std::endl;
    std::cout << se->getName() << std::endl;
    std::cout << we->getName() << std::endl;

    ne->setOwner(*p2);
    se->setOwner(*p2);
    we->setOwner(*p2);

    o->setOwner(*p1);
    q->setOwner(*p1);
    n->setOwner(*p1);

    q->addAdjacent(*o);
    q->addAdjacent(*n);

    for (Territory *f: q->getAdjacencies()) {
        f->setOwner(*p2);
        std::cout << f->getName() << std::endl;
    }

    std::cout << "Continent " << c->getName() << " has bonus armies: " << c->getBonusArmies() << std::endl;
    for (Territory *t: c->getTerritories()) {
        std::cout << "\t" << t->getName() << std::endl;
        for (Territory *a: t->getAdjacencies()) {
            std::cout << "\t\t" << a->getName() << std::endl;
        }
    }
    std::cout << "Continent " << e->getName() << " has bonus armies: " << e->getBonusArmies() << std::endl;
    for (Territory *t: e->getTerritories()) {
        std::cout << "\t" << t->getName() << std::endl;
        for (Territory *a: t->getAdjacencies()) {
            std::cout << "\t\t" << a->getName() << std::endl;
        }
    }

    // Delete territories
    delete q;
    delete o;
    delete n;
    delete ne;
    delete se;
    delete we;

    // Delete Continents
    delete c;
    delete e;

    // Delete Players
    delete p1;
    delete p2;




}