#include <Arduino.h>
#include <MqttClient.h>
#include <WiFi.h>
#include "error.hpp"
#include "constants.h"
#include "network.hpp"
#include "mqtt_esp_client.hpp"

#define HW_UART_SPEED    57600L // Check this 



WifiNetwork* wifiNetwork;
Mqtt_ESP_Client* client;

void setup() {
    Serial.begin(115200, SERIAL_8N1);
    while (!Serial && millis() < 2000) {
        // give USB-CDC a moment so the boot logs aren't lost
    }

    if (strcmp(Constants::MQTT_HOST, "127.0.0.1") == 0 ||
        strcmp(Constants::MQTT_HOST, "localhost") == 0) {
        Serial.println("WARNING: MQTT_HOST is loopback -- that is the ESP32 itself, "
                       "not your broker. Set Constants::MQTT_HOST to the broker's LAN IP.");
    }

    wifiNetwork = new WifiNetwork(Constants::ssid, Constants::pass);
    client = new Mqtt_ESP_Client(wifiNetwork);
}

void loop() {
    client->run_loop();
}
