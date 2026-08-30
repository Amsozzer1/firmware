#pragma once
#include "network.hpp"
class _____System: public MqttClient::System {
    public:
        unsigned long millis() const {
            return ::millis();   // the global Arduino millis()
        }
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
    

    public:
        Mqtt_ESP_Client(WifiNetwork*);
        void setHandler(const char*, void(MqttClient::MessageData&));
};
