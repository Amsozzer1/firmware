#pragma once
#include <WiFi.h>
#include "error.hpp"
#include "constants.h"
#include <MqttClient.h>
class WifiNetwork {
    private:
        IPAddress TEST_SERVER = IPAddress(74,125,115,105);
    public:
        int status;
        WiFiClient wifi;
        WifiNetwork(char ssid[], char password[]) {
            WiFi.begin(ssid, password);
            this->status = WiFi.waitForConnectResult();
            if ( status != WL_CONNECTED) {
                Serial.println("Not Connected to wifi");
                Serial.println(this->status);
            } else {
                Serial.println("Connected to wifi");
                Serial.println("\nStarting connection...");
                if (wifi.connect("google.com", 80)) {
                    Serial.println("connected"); //@TODO: 
                } else {
                    Serial.println("Could not connect to TEST_SERVER");
                }
            }
        }
};

