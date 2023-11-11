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

#ifndef COMP345_LOGGINGOBSERVER_H
#define COMP345_LOGGINGOBSERVER_H

#include <list>
#include <string>

/**
 * Forward declaration
 */
class ILoggable;
class Observer;
class LogObserver;
class Subject;

/**
 * ILoggable is an interface that creates a string to log
 */
class ILoggable {
public:
    virtual ~ILoggable() = default;
    /**
     * Creates a string to log
     * @return the string
     */
    [[nodiscard]] virtual std::string stringToLog() const = 0;
};

/**
 * Abstract class Observer for Observer Pattern
 */
class Observer {
public:
    virtual ~Observer() = default;
    virtual void Update(const ILoggable &loggable) const = 0;
};

/**
 * LogObserver class write notifications to a Log file
 */
class LogObserver final : public Observer {
public:
    explicit LogObserver(const std::string &filename);
    // don't allow copying or assignment due to file manipulation
    LogObserver(const LogObserver &logObserver) = delete;
    LogObserver &operator=(const LogObserver &logObserver) = delete;
    ~LogObserver() override;
    void Update(const ILoggable &loggable) const override;
private:
    std::ofstream *out_;
};

/**
 * Subject class used for Observer pattern
 */
class Subject {
public:
    Subject();
    Subject(const Subject &subject);
    virtual ~Subject();
    virtual void attach(const Observer &observer);
    virtual void detach(const Observer &observer);
    virtual void Notify(const ILoggable &loggable);
private:
    std::list<const Observer *> *observers_;
};

#endif //COMP345_LOGGINGOBSERVER_H
