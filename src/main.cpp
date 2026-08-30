#pragma once
#include <Arduino.h>
#include <MqttClient.h>
#include <WiFi.h>
#include "error.hpp"
#include "constants.h"
#include "network.hpp"
#include "client.h"

#define HW_UART_SPEED    57600L // Check this 

#define MQTT_LOG_ENABLED 1



void setup() {
    Serial.begin(115200, SERIAL_8N1);
    Serial.end();
    Serial.begin(115200, SERIAL_8N1);


    Network* network = new Network(Constants::ssid, Constants::pass);
    Mqtt_ESP_Client client = Mqtt_ESP_Client(network);
    
}

void loop() {

}