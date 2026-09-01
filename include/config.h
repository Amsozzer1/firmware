#pragma once
#include <ArduinoJson.h>
#include "MqttClient.h"
// ONLY A DATA CLASS
class Config {
    public:
        static inline JsonDocument config{};
        static void begin(MqttClient::MessageData& md);
        static inline bool configured {false};

};
