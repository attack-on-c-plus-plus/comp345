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

#include "CommandProcessingDriver.h"

#include "../CommandProcessing.h"
#include "../GameEngine.h"
#include "../Orders.h"

void testCommandProcessor() {
    std::cout << "Testing Command Processor" << std::endl;

    CommandProcessor commandProcessor;
    GameEngine gameEngine{commandProcessor};

    if (Command *command = &commandProcessor.getCommand(gameEngine)) {
        const bool valid = command->validate();
        std::cout << std::boolalpha << "Command valid: " << valid << std::endl;
        std::cout << command->description() << " " << *command << std::endl;
    }
    else {
        std::cout << "Not a valid command" << std::endl;
    }
}
