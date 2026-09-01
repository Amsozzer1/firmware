#pragma once
#include <ArduinoJson.h>
#include "MqttClient.h"
#include "constants.h"
#include "cluster.h"
// ONLY A DATA CLASS

struct pin {
    int enablePin;
    int sensorPin;
};

class Config {
    private:
        static void updatePins();
    public:
        static inline JsonDocument config{};
        static void begin(MqttClient::MessageData& md);
        static inline bool configured {false};
        static inline pin pins[Constants::CLUSTER_SIZE] {};
        static int sharedStepPin;
        static int sharedDirPin;
};
