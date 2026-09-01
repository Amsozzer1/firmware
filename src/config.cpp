#include "config.h"

void Config::begin(MqttClient::MessageData& md) {
    const MqttClient::Message& msg = md.message;

    DeserializationError err = deserializeJson(
        Config::config, (const char*) msg.payload, msg.payloadLen
    );
    
    if (err) {
        Serial.printf("Config: bad setup payload (%s)\n", err.c_str());
        return;
    }

    // @TODO: Update the pin values
    Config::configured = true;
    Serial.print("Config: ");
    serializeJson(Config::config, Serial);
    Serial.println();
}
