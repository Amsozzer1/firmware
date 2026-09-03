#include "config.h"
#include "fault.h"
#include "module.h"
#include "cluster.h"
void Config::apply(const char* json, size_t len) {
    JsonDocument tmp;
    DeserializationError err = deserializeJson(tmp, json, len);
    if (err) { Fault::raise(Fault::CONFIG_UNPROCESSABLE); return; }
    if (!tmp["sharedStepPin"].is<int>() || !tmp["sharedDirPin"].is<int>()) {
        Fault::raise(Fault::CONFIG_UNPROCESSABLE);
        return;
    }
    if (Config::configured) Config::unsetPins();
    Config::config = tmp;
    Config::readPins();
    Config::updatePins();
    Config::configured = true;
}

void Config::releasePin(int pin) {
    if (pin < 0 || pin >= SOC_GPIO_PIN_COUNT) return;
    if (GPIO_IS_VALID_GPIO(pin)) pinMode(pin, 0x00);
}
static bool pinReserved(int pin) {
    if (pin == 0) return true;              // boot strapping
    if (pin == 1 || pin == 3) return true;  // UART0 console
    if (pin >= 6 && pin <= 11) return true; // SPI flash — reconfiguring these bricks the run
    return false;
}

void Config::usePin(int pin) {
    if (pin < 0 || pin >= SOC_GPIO_PIN_COUNT) return;
    if (pinReserved(pin) || !GPIO_IS_VALID_OUTPUT_GPIO(pin)) {
        Fault::raise(Fault::PIN_REFUSED);
        return;
    }
    pinMode(pin, OUTPUT);
}

void Config::useInputPin(int pin) {
    if (pin < 0 || pin >= SOC_GPIO_PIN_COUNT) return;
    if (pinReserved(pin) || !GPIO_IS_VALID_GPIO(pin)) {
        Fault::raise(Fault::PIN_REFUSED);
        return;
    }
    pinMode(pin, INPUT_PULLUP);
}

void Config::readPins() {
    Config::sharedStepPin = Config::config["sharedStepPin"] | -1;
    Config::sharedDirPin  = Config::config["sharedDirPin"]  | -1;

    JsonArray modules = Config::config["modules"];
    int i = 0;
    for (JsonObject m : modules) {
        if (i >= Constants::CLUSTER_SIZE) break;
        Config::pins[i].enablePin = m["enablePin"] | -1;
        Config::pins[i].sensorPin = m["sensorPin"] | -1;
        ++i;
    }
    for (; i < Constants::CLUSTER_SIZE; ++i) Config::pins[i] = pin{};
}

void Config::unsetPins() {
    Cluster::abort();
    // @TODO: ADD SENSOR PIN
    for (int i = 0; i < Constants::CLUSTER_SIZE; ++i) {
        delete Cluster::modules[i];  // parks the driver while its pins are still ours
        Cluster::modules[i] = nullptr;
        Config::releasePin(Config::pins[i].enablePin);
        Config::releasePin(Config::pins[i].sensorPin);
    }
    Config::releasePin(Config::sharedDirPin);
    Config::releasePin(Config::sharedStepPin);
}

void Config::updatePins() {
    Config::usePin(Config::sharedDirPin);
    Config::usePin(Config::sharedStepPin);
    // @TODO: ADD SENSOR PIN
    for (int i = 0; i < Constants::CLUSTER_SIZE; ++i) {
        pin& p = Config::pins[i];
        if (p.enablePin < 0) continue;
        Config::usePin(p.enablePin);
        Config::useInputPin(p.sensorPin);
        Cluster::modules[i] = new Module(p.enablePin, p.sensorPin);
    }
}
