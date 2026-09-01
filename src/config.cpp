#include "config.h"

void Config::apply(const char* json, size_t len) {
    DeserializationError err = deserializeJson(Config::config, json, len);
    if (err) { Serial.printf("Config: bad payload (%s)\n", err.c_str()); return; }
    Config::configured = true;
    Config::updatePins();
}


// @TODO: Update the pin values
void Config::updatePins() {
    for (int i = 0; i<Constants::CLUSTER_SIZE; i++) {

    }
}