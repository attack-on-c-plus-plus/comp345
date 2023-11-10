// Daniel Soldera
// Carson Senthilkumar
// Joe El-Khoury
// Henri Stephane Carbon
// Haris Mahmood

#ifndef COMP345_LOGGINGOBSERVER_H
#define COMP345_LOGGINGOBSERVER_H

#include <string>
#include <list>

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
 * Abstract class Observer for Observer Pattern
 */
class Observer {
public:
    virtual void Update(const ILoggable &loggable) const = 0;
private:

};

/**
 * LogObserver class write notifications to a Log file
 */
class LogObserver : public Observer {
public:
    explicit LogObserver(const std::string &filename);
    // don't allow copying or assignment due to file manipulation
    LogObserver(const LogObserver &logObserver) = delete;
    LogObserver &operator=(const LogObserver &logObserver) = delete;
    ~LogObserver();
    void Update(const ILoggable &loggable) const override;
private:
    std::ofstream *out_;
};

class IObservable {
public:
    virtual void attach(const Observer &observer) = 0;
    virtual void detach(const Observer &observer) = 0;
};

/**
 * Subject class used for Observer pattern
 */
class Subject : virtual public IObservable {
public:
    Subject();
    Subject(const Subject &subject);
    ~Subject();
    void attach(const Observer &observer) override;
    void detach(const Observer &observer) override;
    virtual void Notify(const ILoggable &loggable);
private:
    std::list<const Observer *> *observers_;
};

#endif //COMP345_LOGGINGOBSERVER_H
