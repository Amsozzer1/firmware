#pragma once
#include <ArduinoJson.h>
#include "constants.h"

struct pin { int enablePin; int sensorPin; };

class Config {
    private:
        static void updatePins();
    public:
        static inline JsonDocument config{};
        static inline bool configured {false};
        static inline pin pins[Constants::CLUSTER_SIZE] {};
        static int sharedStepPin;
        static int sharedDirPin;

        static void apply(const char* json, size_t len);   // ← plain bytes
};
