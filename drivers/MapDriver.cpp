#include <iostream>
#include "../Map.h"

void testLoadMaps() {
    std::cout << "Testing map loading..." << std::endl;

    auto *o = new Territory("Ontario");
    auto *q = new Territory("Quebec");
    auto *n = new Territory("New Brunswick");

    auto *p1 = new Player();
    auto *p2 = new Player();

    std::cout << q->getName() << std::endl;
    std::cout << o->getName() << std::endl;
    std::cout << n->getName() << std::endl;

    o->setOwner(*p1);
    q->setOwner(*p1);
    n->setOwner(*p1);

    std::cout << o->getName() << std::endl;
    std::cout << q->getName() << std::endl;
    std::cout << n->getName() << std::endl;

    q->addAdjacent(*o);
    q->addAdjacent(*n);

    const std::vector<Territory *> &a = q->getAdjacencies();

    for (Territory *f: a) {
        f->setOwner(*p2);
        std::cout << f->getName() << std::endl;
    }

    std::cout << o->getName() << std::endl;
    std::cout << q->getName() << std::endl;
    std::cout << n->getName() << std::endl;

    delete q;
    delete o;
    delete n;
    delete p1;
    delete p2;

}