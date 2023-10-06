#ifndef COMP345_DRIVERS_H
#define COMP345_DRIVERS_H

static const char *const mapsPathValid = "maps/valid";

static const char *const mapsPathInvalid = "maps/invalid";

/*
 * Demonstrates that the MapLoader performs its purpose adequately
 */
void testLoadMaps();

/*
    Demonstrates the game state can properly change on command and follows the flow of the game state.
*/
void testGameStates();

/*
 * Demonstrates that Decks, Hand, and Card work as expected
 */
void testCards();

#endif //COMP345_DRIVERS_H
