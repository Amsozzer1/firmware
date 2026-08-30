#include <Arduino.h>
#include <WiFi.h>
#include <MqttClient.h>
#include "error.hpp"
#include "constants.h"
#include "network.hpp"
#include "mqtt_esp_client.hpp"
#include "topicRegistry.hpp"
#define HW_UART_SPEED    57600L // Check this 


WifiNetwork* wifiNetwork;
Mqtt_ESP_Client* client;

void setup() {
    Serial.begin(115200, SERIAL_8N1);
    while (!Serial && millis() < 2000) {
        // give USB-CDC a moment so the boot logs aren't lost
    }
    TopicRegistry::begin();
    wifiNetwork = new WifiNetwork(Constants::ssid, Constants::pass);
    client = new Mqtt_ESP_Client(wifiNetwork);
}

void loop() {
    client->run_loop();
}
