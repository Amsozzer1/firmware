#pragma once
#include <ArduinoJson.h>
#include "constants.h"

struct pin { int enablePin{-1}; int sensorPin{-1}; };

class Config {
    private:
        static void updatePins();
        static void unsetPins();

    public:
        static void releasePin(int);
        static void usePin(int);
        static inline JsonDocument config{};
        static inline bool configured {false};
        static inline pin pins[Constants::CLUSTER_SIZE] {};
        inline static int sharedStepPin {-1};
        inline static int sharedDirPin {-1};

        static void apply(const char* json, size_t len);
};
