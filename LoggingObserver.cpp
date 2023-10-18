// Daniel Soldera
// Carson Senthilkumar
// Joe El-Khoury
// Henri Stephane Carbon
// Haris Mahmood

#include "LoggingObserver.h"

Subject::Subject() {
    observers_ = new std::list<const Observer *>();
}

Subject::~Subject() {
    delete observers_;
}

void Subject::attach(const Observer &observer) {
    observers_->push_back(&observer);
}

void Subject::detach(const Observer &observer) {
    observers_->remove(&observer);
}

void Subject::Notify(const ILoggable &loggable) {
    for (auto it = observers_->begin(); it != observers_->end(); ++it) {
        // TODO: call update
    }
}

Subject::Subject(const Subject &subject) {
    observers_ = new std::list<const Observer *>(*subject.observers_);
}
