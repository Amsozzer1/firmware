#include <Arduino.h>
#include <MqttClient.h>
#include <WiFi.h>
#include "error.hpp"
#include "constants.h"
#include "network.hpp"
#include "mqtt_esp_client.hpp"

#define HW_UART_SPEED    57600L // Check this 




void setup() {
    Serial.begin(115200, SERIAL_8N1);
    Serial.end();
    Serial.begin(115200, SERIAL_8N1);


    WifiNetwork* wifiNetwork = new WifiNetwork(Constants::ssid, Constants::pass);
    Mqtt_ESP_Client client = Mqtt_ESP_Client(wifiNetwork);
    
}

void loop() {

}