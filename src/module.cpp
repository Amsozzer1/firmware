/*
    A Module is a single spool driver
    Jobs:
        - load filament
        - un-load filament
        - Edge Cases & Error Handling for the real BE

    Components:
        - TMC2209 - Driver
        - NEMA 17 - Stepper Motor
        - Filament Sensor

    Step and dir are shared across the cluster, so the module's own enable pin
    is the only thing that decides which motor actually turns. It is asserted
    for the length of a move and released the moment the move ends.

    TRAVEL TIME ANOMALY  (nice-to-have)
        Time between the two sensors. Track the running average,
        set a threshold.
        On anomaly: stop the module, error to backend -- "go check".
        User says it's fine -> set average to null.
        Average null -> we don't reason about travel time.

*/

#include <Arduino.h>
#include "module.h"
#include "config.h"
#include "fault.h"

Module::Module(int enable, int sensor) {
    this->enablePin = enable;
    this->sensorPin = sensor;
    this->job = Job::NONE;
    this->engaged = false;
    this->start = 0;
    digitalWrite(this->enablePin, HIGH);  // TMC2209 EN is active low, so HIGH is parked
}

Module::~Module() {
    this->stop();
}

bool Module::sensedFilamentInPrinter() {
    return Module::filemantSensedInPrinter || false;
}

bool Module::sensedFilament() {
    // @TODO: sense filament - we read this from the report
    return false;
}

/*
    LOAD  (filament starts before the module sensor)
    A) cross module sensor. If not: out of filament, or stuck.
    B) keep feeding to printer sensor -> stop.
    First is a health check. Second is where we actually stop.

    UNLOAD is the same walk backwards: leave the printer sensor, then run
    until the module sensor lets go.
*/
void Module::load()   { this->begin(Job::LOAD, HIGH); }
void Module::unLoad() { this->begin(Job::UNLOAD, LOW); }

void Module::begin(Job job, int dir) {
    digitalWrite(Config::sharedDirPin, dir);
    digitalWrite(Config::sharedStepPin, LOW);
    digitalWrite(this->enablePin, LOW);
    this->job = job;
    this->engaged = false;
    this->start = millis();
}

bool Module::crossed() {
    return this->job == Job::LOAD ? this->sensedFilament() : !this->sensedFilamentInPrinter();
}

bool Module::arrived() {
    return this->job == Job::LOAD ? this->sensedFilamentInPrinter() : !this->sensedFilament();
}

// Advances the move by a bounded burst of steps. Returns false once the move
// is over -- finished or faulted -- and the caller drops us as the active one.
bool Module::tick() {
    if (this->job == Job::NONE) return false;

    for (int i = 0; i < Constants::STEPS_PER_TICK; ++i) {
        if (this->arrived()) { this->stop(); return false; }

        if (!this->engaged) {
            if (this->crossed()) {
                this->engaged = true;
                this->start = millis();
            } else if (millis() - this->start > Constants::ENGAGE_TIMEOUT_MS) {
                Fault::raise(Fault::LOAD_TIMEOUT);
                this->stop();
                return false;
            }
        } else if (millis() - this->start > Constants::LOAD_TIMEOUT_MS) {
            Fault::raise(Fault::LOAD_TIMEOUT);
            this->stop();
            return false;
        }

        this->pulse();
    }
    return true;
}

void Module::pulse() {
    digitalWrite(Config::sharedStepPin, HIGH);
    delayMicroseconds(Constants::PLUS_FREQUENCY_MS);
    digitalWrite(Config::sharedStepPin, LOW);
    delayMicroseconds(Constants::PLUS_FREQUENCY_MS);
}

void Module::stop() {
    digitalWrite(Config::sharedStepPin, LOW);
    digitalWrite(this->enablePin, HIGH);
    this->job = Job::NONE;
    this->engaged = false;
}

void Module::setFilamentSensedInPrinter(const char *json, size_t len) {
    JsonDocument request;

    DeserializationError err = deserializeJson(request, json, len);
    if (err) {
        // @TODO: add a better error code here
        Fault::raise(Fault::REQ_UNPROCESSABLE);
        return;
    }
    const JsonVariant value = request["sensed"];
    if (!value.is<bool>()) {
        // @TODO: add a better error code here
        Fault::raise(Fault::REQ_UNPROCESSABLE);
        return;
    }

    Module::filemantSensedInPrinter=value;
}