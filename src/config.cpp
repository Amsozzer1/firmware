#include "config.h"
#include "fault.h"
void Config::apply(const char* json, size_t len) {
    JsonDocument tmp;
    DeserializationError err = deserializeJson(tmp, json, len);
    if (err) { Fault::raise(Fault::CONFIG_UNPROCESSABLE); }
    if (Config::configured) Config::unsetPins();
    Config::config = tmp;
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


// @TODO: Unset the pin values
void Config::unsetPins() {
    Config::releasePin(Config::sharedDirPin);
    Config::releasePin(Config::sharedStepPin);
    // @TODO: ADD SENSOR PIN
    for (auto& p : Config::pins) {
        // @TODO: CLEAN UP MODULES 
        Config::releasePin(p.enablePin);
        Config::releasePin(p.sensorPin);
    }
}

// @TODO: Update the pin values
void Config::updatePins() {
    Config::usePin(Config::sharedDirPin);
    Config::usePin(Config::sharedStepPin);
    // @TODO: ADD SENSOR PIN
    for (auto& p : Config::pins) {
        // @TODO: SETUP UP MODULES
        Config::usePin(p.enablePin);
        Config::usePin(p.sensorPin);
    }
}