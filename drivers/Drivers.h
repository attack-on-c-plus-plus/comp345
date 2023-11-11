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

#ifndef COMP345_DRIVERS_H
#define COMP345_DRIVERS_H

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
