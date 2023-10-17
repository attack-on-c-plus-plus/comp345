// Daniel Soldera
// Carson Senthilkumar
// Joe El-Khoury
// Henri Stephane Carbon
// Haris Mahmood

#ifndef COMP345_LOGGINGOBSERVER_H
#define COMP345_LOGGINGOBSERVER_H

#include <string>

/**
 * ILoggable is an interface that creates a string to log
 */
class ILoggable {
public:
    /**
     * Creates a string to log
     * @return the string
     */
    [[nodiscard]] virtual std::string stringToLog() const = 0;
};

#endif //COMP345_LOGGINGOBSERVER_H
