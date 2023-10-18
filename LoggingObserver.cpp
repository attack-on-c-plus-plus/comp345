// Daniel Soldera
// Carson Senthilkumar
// Joe El-Khoury
// Henri Stephane Carbon
// Haris Mahmood

#include "LoggingObserver.h"

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
    observers_ = new std::list<const Observer *>(*subject.observers_);
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
    for (auto it = observers_->begin(); it != observers_->end(); ++it) {
        // TODO: call Observer update method
    }
}
