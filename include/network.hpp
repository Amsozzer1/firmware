#pragma once
#include <WiFi.h>
#include "constants.h"
#include <MqttClient.h>

class WifiNetwork {
    private:
        int status;

    public:
        // The MqttClient wraps this socket by reference; it never opens it
        // itself, so connectBroker() has to be the one to dial the broker.
        WiFiClient wifi;

        WifiNetwork(const char ssid[], const char password[])
            : status(WL_IDLE_STATUS), mSsid(ssid), mPass(password) {
            connect();
        }

        bool isConnected() const { return WiFi.status() == WL_CONNECTED; }

        // Associates with the AP. No-op when already associated, so callers can
        // poll this every loop without tearing down a working link.
        bool connect() {
            if (isConnected()) {
                return true;
            }
            WiFi.mode(WIFI_STA);
            WiFi.begin(mSsid, mPass);
            this->status = WiFi.waitForConnectResult(Constants::WIFI_CONNECT_TIMEOUT_MS);
            if (this->status != WL_CONNECTED) {
                Serial.printf("WiFi: not connected, status %d\n", this->status);
                return false;
            }
            Serial.printf("WiFi: connected, ip %s\n", WiFi.localIP().toString().c_str());
            return true;
        }

        void disconnect() {
            wifi.stop();
            WiFi.disconnect();
            this->status = WL_IDLE_STATUS;
        }

        bool brokerConnected() { return wifi.connected(); }

        // TCP socket to the broker, which the MqttClient then speaks over.
        bool connectBroker() {
            if (brokerConnected()) {
                return true;
            }
            if (!isConnected()) {
                return false;
            }
            Serial.printf("TCP: connecting to %s:%d\n", Constants::MQTT_HOST, Constants::MQTT_PORT);
            if (!wifi.connect(Constants::MQTT_HOST, Constants::MQTT_PORT)) {
                Serial.println("TCP: broker unreachable");
                return false;
            }
            Serial.println("TCP: connected");
            return true;
        }

        void disconnectBroker() { wifi.stop(); }

    private:
        const char* mSsid;
        const char* mPass;
};
