#include "client.h"

Mqtt_ESP_Client::Mqtt_ESP_Client(WifiNetwork* network) {
    this->mqttSystem = new _____System();
    this->mqttNetwork = new MqttClient::NetworkClientImpl<Client>(network->wifi, *mqttSystem);
    this->mqttLogger = new MqttClient::LoggerImpl<HardwareSerial>(Serial);
    this->mqttSendBuffer = new MqttClient::ArrayBuffer<128>();
    this->mqttRecvBuffer = new MqttClient::ArrayBuffer<128>();
    this->mqttMessageHandlers = new MqttClient::MessageHandlersImpl<2>();
    this->mqttOptions.commandTimeoutMs = 10000;

    this->mqtt = new MqttClient (
		this->mqttOptions,
        *this->mqttLogger, 
        *this->mqttSystem,
        *this->mqttNetwork,
        *this->mqttSendBuffer,
        *this->mqttRecvBuffer, 
        *this->mqttMessageHandlers
	);

};

void Mqtt_ESP_Client::setHandler(const char* topic, void handler(MqttClient::MessageData&) ) {
    this->mqttMessageHandlers->set(topic, handler);
}