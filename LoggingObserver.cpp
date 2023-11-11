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

#include "LoggingObserver.h"

#include <fstream>

/**
 * Constructor
 */
Subject::Subject() {
    observers_ = new std::list<const Observer *>();
}

/**
 * Copy constructor
 * @param subject
 */
Subject::Subject(const Subject &subject) {
    observers_ = new std::list(*subject.observers_);
}

/**
 * Destructor
 */
Subject::~Subject() {
    delete observers_;
}

/**
 * Attaches a new Observer to be notified
 * @param observer
 */
void Subject::attach(const Observer &observer) {
    observers_->push_back(&observer);
}

/**
 * Detaches an Observer so that it stops being notified
 * @param observer
 */
void Subject::detach(const Observer &observer) {
    observers_->remove(&observer);
}

/**
 * Notifies all attached Observers
 * @param loggable
 */
void Subject::Notify(const ILoggable &loggable) {
    for (const auto & observer : *observers_) {
        observer->Update(loggable);
    }
}

LogObserver::LogObserver(const std::string &filename) {
    out_ = new std::ofstream(filename);
}

LogObserver::~LogObserver() {
    if (out_->is_open()) {
        *out_ << std::string(70, '=') << std::endl;
    }
    delete out_;
}

void LogObserver::Update(const ILoggable &loggable) const {
    if (out_->is_open()) {
        *out_ << std::string(70, '=') << std::endl;
        *out_ << loggable.stringToLog() << std::endl;
    }
}
