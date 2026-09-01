#pragma once
#include "network.hpp"
#include "config.h"
#include "topicRegistry.hpp"
#include "cluster.h"
class _____System: public MqttClient::System {
    public:
        unsigned long millis() const {
            return ::millis();   // the global Arduino millis()
        }
        // @TODO: add the yield function here
};

class Mqtt_ESP_Client {

    WifiNetwork* network; // WIFI network
    MqttClient* mqtt; // the real mqtt client
    MqttClient::Network* mqttNetwork;
    MqttClient::System *mqttSystem;
    MqttClient::Logger *mqttLogger;
    MqttClient::Buffer *mqttSendBuffer;
    MqttClient::Buffer *mqttRecvBuffer;
    MqttClient::MessageHandlers *mqttMessageHandlers;

    MqttClient::Options mqttOptions;

    bool hasAttemptedConnect;
    unsigned long lastConnectAttemptMs;
    unsigned long lastPublishMs;
    bool reconnect();

    public:
        Mqtt_ESP_Client(WifiNetwork*);
        void setHandler(const char*, void(MqttClient::MessageData&));
        void run_loop();
};
