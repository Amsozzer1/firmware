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


    TRAVEL TIME ANOMALY  (nice-to-have)
        Time between the two sensors. Track the running average,
        set a threshold.
        On anomaly: stop the module, error to backend -- "go check".
        User says it's fine -> set average to null.
        Average null -> we don't reason about travel time.

*/

#include <Arduino.h>
#include <chrono>
#include "error.cpp"
#define SHARED_DIR_PIN 21
#define SHARED_STEP_PIN 22
#define LOAD_TIMEOUT_MS 5000
#define ENGAGE_TIMEOUT_MS 5000
class Module {
    private:
        int enable;
        int id;
        /*
            Sensors:
                module sensor   -- per slot, past the drive gear
                printer sensor  -- at the printer
        */
        bool Module::sensedFilamentInPrinter() {
            // @TODO: sense filament
            return true;
        }
        bool Module::sensedFilament() {
            // @TODO: sense filament
            return true;
        }
    public:
        Module::Module(int pin) {
            this->enable = LOW;
            this->id = pin;
        }

        void Module::load() {
            /*
                LOAD  (filament starts before the module sensor)
                A) cross module sensor. If not: out of filament, or stuck.
                B) keep feeding to printer sensor -> stop.
                First is a health check. Second is where we actually stop.
            */
            // SETUP
            digitalWrite(SHARED_STEP_PIN, LOW);

            digitalWrite(SHARED_DIR_PIN, HIGH);
            bool engaged = false;
            uint32_t start = millis();
            while (!sensedFilamentInPrinter()) {
                if (!engaged) {
                    if (sensedFilament()) {
                        engaged = true;
                        start = millis();
                    } else if (millis() - start > ENGAGE_TIMEOUT_MS) {
                        AppErrorInit init;
                        init.status = 500;
                        init.code = "!ENGAGED";
                        throw ModuleError("Filament not engaged with Module Filament Sensor", init);
                    }
                } else if (millis() - start > LOAD_TIMEOUT_MS) {
                    AppErrorInit init;
                    init.status = 500;
                    init.code = "TIMEOUT";
                    throw ModuleError("Filament swap took too long", init);
                };
                digitalWrite(SHARED_STEP_PIN, HIGH);
                delay(500);
            }
            
        }

        void Module::unLoad() {
            /*
                UNLOAD
                A) health check: filament leaves the printer within threshold.
                If not, throw an error.
                B) keep unloading until the module sensor stops sensing it.
            */
            // SETUP
            digitalWrite(SHARED_STEP_PIN, LOW);

            digitalWrite(SHARED_DIR_PIN, LOW);
            while (!this->sensedFilament()) {
                digitalWrite(SHARED_STEP_PIN, LOW);
                delay(500);
            }
        }
};