#include "mqtt_esp_client.hpp"

Mqtt_ESP_Client::Mqtt_ESP_Client(WifiNetwork* net) {
    // This assignment was missing, so `this->network` held whatever `new` left
    // behind and the first run_loop() faulted on it.
    this->pinsConfigured = false;
    this->network = net;

    this->mqttSystem = new _____System();
    this->mqttNetwork = new MqttClient::NetworkClientImpl<Client>(net->wifi, *mqttSystem);
    this->mqttLogger = new MqttClient::LoggerImpl<HardwareSerial>(Serial);
    this->mqttSendBuffer = new MqttClient::ArrayBuffer<Constants::BUFFER_SIZE>();
    this->mqttRecvBuffer = new MqttClient::ArrayBuffer<Constants::BUFFER_SIZE>();
    this->mqttMessageHandlers = new MqttClient::MessageHandlersImpl<2>();
    this->mqttOptions.commandTimeoutMs = 10000;

    this->hasAttemptedConnect = false;
    this->lastConnectAttemptMs = 0;
    this->lastPublishMs = 0;

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

// Brings the whole stack up: AP association -> TCP to the broker -> MQTT
// CONNECT -> subscribe. Returns false at the first step that fails so the
// caller can back off and retry.
bool Mqtt_ESP_Client::reconnect() {
    // Start from a known state; a half-open socket makes CONNECT hang until
    // commandTimeoutMs rather than fail fast.
    if (this->mqtt->isConnected()) {
        this->mqtt->disconnect();
    }
    this->network->disconnectBroker();

    if (!this->network->connect()) {
        return false;
    }
    if (!this->network->connectBroker()) {
        return false;
    }

    {
        MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
        options.MQTTVersion = 4;
        options.clientID.cstring = (char*) Constants::MQTT_ID;
        options.cleansession = true;
        options.keepAliveInterval = 15;

        MqttClient::ConnectResult connectResult;
        MqttClient::Error::type rc = this->mqtt->connect(options, connectResult);
        if (rc != MqttClient::Error::SUCCESS) {
            Serial.printf("MQTT: connect error %i\n", rc);
            this->network->disconnectBroker();
            return false;
        }
    }

    // SUBSCRIBING to ESP Setup - this is config setup
    {
        MqttClient::Error::type rc = this->mqtt->subscribe(TopicRegistry::espSetup(), MqttClient::QOS2);
        if (rc != MqttClient::Error::SUCCESS) {
            Serial.printf("MQTT: subscribe error %i, dropping connection\n", rc);
            this->mqtt->disconnect();
            this->network->disconnectBroker();
            return false;
        }
    }

    Serial.printf("MQTT: connected, subscribed to %s\n", TopicRegistry::espSetup());
    return true;
}
bool Mqtt_ESP_Client::setupPins(Config pinConf) {
    if (this->pinsConfigured) return true;
    /* @TODO: Takes the config from MQTT and setups the pins for example
        digitalWrite(PIN, OUTPUT);
    */
    return false;

}
void Mqtt_ESP_Client::run_loop() {
    const unsigned long now = millis();

    // isConnected() only reports the MQTT session flag, so check the socket too
    // -- otherwise a dropped TCP link looks healthy and we publish into a void.
    if (!this->mqtt->isConnected() || !this->network->brokerConnected()) {
        if (this->hasAttemptedConnect &&
            (now - this->lastConnectAttemptMs) < Constants::MQTT_RECONNECT_BACKOFF_MS) {
            return;
        }
        this->hasAttemptedConnect = true;
        this->lastConnectAttemptMs = now;
        this->reconnect();
        
        return;
    }

    //Config pinConf; // @TODO: This will be read from MQTT setup Topic
    // Also if config isnt set, hit the brain to tell it that we need the config maybe? or should the broker release the 
    //if (!this->setupPins(pinConf)) { /* @TODO add the correct way to throw once conf*/ }
    // @TODO: use TopicRegistry::setPrinter(char* printerId) once pinConf is setup
    
    // @TODO:  this is just test, DUHH, create the real implementation
    if ((now - this->lastPublishMs) >= Constants::PUBLISH_INTERVAL_MS) {
        this->lastPublishMs = now;
        const char* buf = "Hello";
        MqttClient::Message message;
        message.qos = MqttClient::QOS0;
        message.retained = false;
        message.dup = false;
        message.payload = (void*) buf;
        message.payloadLen = strlen(buf);
        this->mqtt->publish(TopicRegistry::espReport(), message);
    }


    // @TODO: this should be it 
    // if (!this->pinsConfigured) this->mqtt->yield(500);
    this->mqtt->yield(500);
}
