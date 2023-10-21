// Daniel Soldera
// Carson Senthilkumar
// Joe El-Khoury
// Henri Stephane Carbon
// Haris Mahmood

#ifndef COMP345_LOGGINGOBSERVER_H
#define COMP345_LOGGINGOBSERVER_H

#include <string>
#include <list>

class Observer;

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

/**
 * Subject class used for Observer pattern
 */
class Subject {
public:
    Subject();
    Subject(const Subject &subject);
    ~Subject();
    virtual void attach(const Observer &observer);
    virtual void detach(const Observer &observer);
    virtual void Notify(const ILoggable &loggable);
private:
    std::list<const Observer *> *observers_;
};

#endif //COMP345_LOGGINGOBSERVER_H
