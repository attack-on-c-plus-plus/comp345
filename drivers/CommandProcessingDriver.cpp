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
    const std::string seperator(70, '=');
    std::cout << seperator << std::endl;
    std::cout << "Testing Command Processor" << std::endl;
    std::cout << seperator << std::endl;

    FakeRandom random;
    CommandProcessor commandProcessor;
    GameEngine gameEngine1{commandProcessor, random};

    for (int i = 0; i < 6; ++i) {
        if (Command *command = &commandProcessor.getCommand(gameEngine1)) {
            const bool valid = command->validate();
            std::cout << std::boolalpha << "Command valid: " << valid << std::endl;
            command->execute();
            std::cout << command->description() << " " << *command << std::endl;
        }
        else {
            std::cout << "Not a valid command" << std::endl;
        }
    }

    std::cout << seperator << std::endl;
    std::cout << "Testing FileCommand Processor Adapter" << std::endl;
    std::cout << seperator << std::endl;

    FileCommandProcessorAdapter fileCommandProcessorAdapter{"res/commandTests.txt"};
    GameEngine gameEngine2{fileCommandProcessorAdapter, random};

    for (int i = 0; i < 6; ++i) {
        if (Command *command = &fileCommandProcessorAdapter.getCommand(gameEngine2)) {
            const bool valid = command->validate();
            std::cout << std::boolalpha << "Command valid: " << valid << std::endl;
            command->execute();
            std::cout << command->description() << " " << *command << std::endl;
        }
        else {
            std::cout << "Not a valid command" << std::endl;
        }
    }
}
