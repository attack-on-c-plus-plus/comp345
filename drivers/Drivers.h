#ifndef COMP345_DRIVERS_H
#define COMP345_DRIVERS_H

// Daniel Soldera
// Carson Senthilkumar
// Joe El-Khoury
// Henri Stephane Carbon
// Haris Mahmood

static const char *const mapsPathValid = "maps/valid";

static const char *const mapsPathInvalid = "maps/invalid";

/*
 * Demonstrates that the MapLoader performs its purpose adequately
 */
void testLoadMaps();

/*
 * Demonstrates the player capabilities, collection of hand, territories and retrieving
 * information.
 */
void testPlayer();

/**
 * Demonstrates the game state can properly change on command and follows the flow of
 * the game state.
*/
//void testGameStates();
void testGameStartup();


/*
 * Demonstrates that Decks, Hand, and Card work as expected
 */
void testCards();

void testOrderList();

void testLoggingObserver();

#endif //COMP345_DRIVERS_H
