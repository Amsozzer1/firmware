#include <Arduino.h>
#include <WiFi.h>
#include <MqttClient.h>
#include "constants.h"
#include "network.hpp"
#include "mqtt_esp_client.h"
#include "topicRegistry.hpp"
#define HW_UART_SPEED    57600L // Check this 


WifiNetwork* wifiNetwork;
Mqtt_ESP_Client* client;

void onSetup(MqttClient::MessageData& md) {
    Config::apply((const char*)md.message.payload, md.message.payloadLen);
}

void setup() {
    Serial.begin(115200, SERIAL_8N1);
    while (!Serial && millis() < 2000) {
        // give USB-CDC a moment so the boot logs aren't lost
    }

    // @TODO: add guards here
    TopicRegistry::begin();
    
    wifiNetwork = new WifiNetwork(Constants::ssid, Constants::pass);
    client = new Mqtt_ESP_Client(wifiNetwork);

    client->setHandler(TopicRegistry::espSetup(), onSetup);
    client->setHandler(TopicRegistry::espRequest(), Request::handleRequest);

    Serial.printf("Your Mac Address is %s \n", TopicRegistry::macStr());
}

void loop() {
    client->run_loop();
}
