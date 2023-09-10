# COMP 345 Assignment 1

## Grading Criteria

### Map

#### Knowledge/Correctness of Game Rules.

1. Students are fully aware of the correct Warzone game rules to implement during the presentation, and the code is implemented according to the rules of the game.

#### Compliance of solution with stated problem.

1. Territories contain a pointer to a player that owns the territory.
2. Territories contain army units.
3. Presence of a Map::validate() method.
4. Map is a collection of Territories.
5. Continents are a subset of the Territories in the graph.
6. Map can be made to represent any map configuration: MapLoader can load a variety of map files.
7. Map is implemented using a graph data structure with Territories as nodes, and pointers to Territories as edges.
8. All data members of user-defined class type are of pointer type.
9. Classes declared in header file. Functions implemented in cpp file. Absence of inline functions.
10. All classes implement a correct copy constructor, assignment operator, and stream insertion operator.
11. Absence of memory leaks.
12. Driver demonstrates that the Map::validate() method checks that: 1) a map is a connected graph 2) a continent is a connected subgraph 3) each country belongs to one and only one continent.

#### Modularity of Solution

1. All is implemented in the file duo named Map.cpp/Map.h, and no other files.
2. Presence of classes named Territory, Map, and MapLoader; usage of class Player to signify Territory ownership.

#### Mastery of Language/Tools/Libraries.

1. The program never crashes during the demonstration or code review.
2. Students were very clear in technical discussion during the demonstration.

#### Code readability: name conventions, clarity of code, use of comments.

1. All user-defined classes, methods, free functions, and operators are documented.
2. Clear/consistent naming convention is used.
3. Absence of commented-out code.

### Player

#### Knowledge/Correctness of Game Rules.

1. Students are fully aware of the correct Warzone game rules to implement during the presentation, and the code is implemented according to the rules of the game.

#### Compliance of solution with stated problem.

1. A player includes a collection of pointers to territories - using the Territory class
2. A player includes a hand of cards - using the Hand class
3. A player includes a list of orders - using the OrderList class
4. Player class includes toAttack and toDefend methods that return a list of orders.
5. Player class includes an issueOrder method that creates an Order object and places it in the list of orders of the player
6. All data members of user-defined class type are of pointer type.
7. Classes declared in header file. Function implemented in cpp file. Absence of inline functions.
8. All classes implement a correct copy constructor, assignment operator, and stream insertion operator.
9. Absence of memory leaks.
10. Driver clearly demonstrates that toAttack() and toDefend() can be called and return list of Territories.
11. Driver clearly demonstrates that issueOrder() can be called and will result in an order to be placed in the list of orders of the player.
12. Driver clearly demonstrates that a player stores a hand of cards.
13. Driver clearly demonstrates that a player stores a list of owned territories.

#### Modularity of Solution

1. All is implemented in the file duo named Player.cpp/Player.h, and no other files.
2. Presence of Player class; usage of classes Territory, Hand and OrderList.

#### Mastery of Language/Tools/Libraries.

1. The program never crashes during the demonstration or code review.
2. Students were very clear in technical discussion during the demonstration.

#### Code readability: name conventions, clarity of code, use of comments.

1. All user-defined classes, methods, free functions, and operators are documented.
2. Clear/consistent naming convention is used.
3. Absence of commented-out code.

### OrderList

#### Knowledge/Correctness of Game Rules.

1. Students are fully aware of the correct Warzone game rules to implement during the presentation, and the code is implemented according to the rules of the game.

#### Compliance of solution with stated problem.

1. Orders of any subclass can be created by the player and placed sequentially in the list of orders
2. Orders can be moved in the list using the move() method
3. Orders can be removed from the list using the remove() method
4. Orders have an execute() method.
5. All data members of user-defined class type are of pointer type.
6. Classes declared in header file. Function implemented in cpp file. Absence of inline functions.
7. All classes implement a correct copy constructor, assignment operator, and stream insertion operator.
8. Absence of memory leaks.
9. Driver clearly demonstrates that orders of every kind can be created and placed in an OrderList object.
10. Driver clearly demonstrates that orders can be moved and deleted using move() and remove()
11. Driver clearly demonstrates that orders can be validated and executed using validate() and execute()

#### Modularity of Solution

1. All is implemented in the file duo named Orders.cpp/Orders.h, and no other files.
2. Presence of OrderList, and Order classes. Presence of Deploy, Advance, Bomb, Blockade, Airlift, and Negotiate as subclasses of the Order class.

#### Mastery of Language/Tools/Libraries.

1. The program never crashes during the demonstration or code review.
2. Students were very clear in technical discussion during the demonstration.

#### Code readability: name conventions, clarity of code, use of comments.

1. All user-defined classes, methods, free functions, and operators are documented.
2. Clear/consistent naming convention is used.
3. Absence of commented-out code.

### Cards deck/hand

#### Knowledge/Correctness of Game Rules.

1. Students are fully aware of the correct Warzone game rules to implement during the presentation, and the code is implemented according to the rules of the game.

#### Compliance of solution with stated problem.

1. A Deck class contains a collection of Cards
2. A Deck has a method draw() that removes the card from the Deck and returns that card
3. A Hand class contains a collection of Cards
4. A Card class has a play() method, which creates an Order object and places it in the Players’ list of orders, removes the Card from the Hand, and places it back in the Deck.
5. All data members of user-defined class type are of pointer type.
6. Classes declared in header file. Function implemented in cpp file. Absence of inline functions.
7. All classes implement a correct copy constructor, assignment operator, and stream insertion operator.
8. Absence of memory leaks.
9. Driver clearly demonstrates that orders of every kind can be created and placed in an OrderList object.
10. Driver clearly demonstrates that orders can be moved and deleted using move() and remove()
11. Driver clearly demonstrates that orders can be validated and executed using validate() and execute()

#### Modularity of Solution

1. All is implemented in the file duo named Cards.cpp/Cards.h, and no other files.
2. Presence of classes named Card, Deck, and Hand; usage of classes Player, Order, and OrderList.

#### Mastery of Language/Tools/Libraries.

1. The program never crashes during the demonstration or code review.
2. Students were very clear in technical discussion during the demonstration. 

#### Code readability: name conventions, clarity of code, use of comments.

1. All user-defined classes, methods, free functions, and operators are documented.
2. Clear/consistent naming convention is used.
3. Absence of commented-out code.

### Game engine

#### Knowledge/Correctness of Game Rules.

1. Students are fully aware of the correct Warzone game rules to implement during the presentation, and the code is implemented according to the rules of the game.

#### Compliance of solution with stated problem.

1. The state of the game is held as a data member of the GameEngine class.
2. Console input. Is used to get commands from the user.
3. Depending on the command and the state, a transition is made to another state by changing the value held by the state variable.
4. If the user enters an invalid command for the current state, the command is rejected and an error message is output to the console.
5. The state transitions are implemented exactly as depicted in the assignment handout.
6. All data members of user-defined class type are of pointer type.
7. Classes declared in header file. Function implemented in cpp file. Absence of inline functions.
8. All classes implement a correct copy constructor, assignment operator, and stream insertion operator.
9. Absence of memory leaks.
10. Driver clearly demonstrates that commands can be used to navigate through the different states as depicted in the assignment handout.

#### Modularity of Solution

1. All is implemented in the file duo named GameEngine.cpp/GameEngine.h, and no other files.
2. Presence of GameEngine class.

#### Mastery of Language/Tools/Libraries.

1. The program never crashes during the demonstration or code review.
2. Students were very clear in technical discussion during the demonstration.

#### Code readability: name conventions, clarity of code, use of comments.

1. All user-defined classes, methods, free functions, and operators are documented.
2. Clear/consistent naming convention is used.
3. Absence of commented-out code.