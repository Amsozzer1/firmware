#pragma once
#include <ArduinoJson.h>
#include "MqttClient.h"
// ONLY A DATA CLASS
class Config {
    public:
        static inline JsonDocument config{};
        void processMessage(MqttClient::MessageData& md);
        static void begin(MqttClient::MessageData& md);

};
