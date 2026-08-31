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

// @TODO: THIS IS TEMP
// void processMessage(MqttClient::MessageData& md) {
//     Serial.println("Cool new message came in BUDDY");
// 	const MqttClient::Message& msg = md.message;
// 	char* payload;
// 	memcpy(payload, msg.payload, msg.payloadLen);
// 	payload[msg.payloadLen] = '\0';
// 	Serial.printf(
// 		"Message arrived: qos %d, retained %d, dup %d, packetid %d, payload:[%s]\n",
// 		msg.qos, msg.retained, msg.dup, msg.id, payload
// 	);
// }
Config* conf;
void processMessage(MqttClient::MessageData& md) {
    conf->processMessage(md);
}
void setup() {
    Serial.begin(115200, SERIAL_8N1);
    while (!Serial && millis() < 2000) {
        // give USB-CDC a moment so the boot logs aren't lost
    }
    conf = new Config();

    TopicRegistry::begin();
    wifiNetwork = new WifiNetwork(Constants::ssid, Constants::pass);
    client = new Mqtt_ESP_Client(wifiNetwork);

    client->setHandler(TopicRegistry::espSetup(), Config::begin);
    Serial.println(TopicRegistry::espSetup());
}

void loop() {
    client->run_loop();
}
