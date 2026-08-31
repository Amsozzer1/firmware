#pragma once
#include <stdint.h>
#include <esp_mac.h>

class TopicRegistry {
public:
    static void begin() {
        esp_read_mac(mac_, ESP_MAC_WIFI_STA);
        for (int i = 0; i < 6; ++i) {
            macStr_[i * 2]     = hex(mac_[i] >> 4);
            macStr_[i * 2 + 1] = hex(mac_[i] & 0x0F);
        }
        macStr_[12] = '\0';

        build(espSetup_, "esp/", "/setup"); // THIS CHANNEL IS ONLY FOR SETUP AND EVERY LISTEN ON THIS CAN MEAN RESTARTING THE ESP
        build(espReport_,     "esp/",     "/report"); // PUBLISH active modules state 
        build(espRequest_,    "esp/",     "/request"); // Read this to know when we get load/unload command is recieved 
    }

    static void setPrinter(char* printerId) {
        build(printerReport_, "printer/", "/report", printerId); // to read printer state -> filamentPresenceSensor
    }
    static const uint8_t* mac()    { return mac_; }
    static const char*    macStr() { return macStr_; }

    // Subscribe to this channel to read Printer State
    static const char* printerReport() { return printerReport_; }
    // Publish to this channel to write the ESP32 State
    static const char* espReport()     { return espReport_; }
    static const char* espRequest()    { return espRequest_; }
    static const char* espSetup()      { return espSetup_; }

private:
    static constexpr size_t kMaxTopic = 64;

    static void build(char* out, const char* prefix, const char* suffix, const char* id=macStr_) {
        char* w = out;
        while (*prefix) *w++ = *prefix++;
        for (const char* s = id; *s; ++s) *w++ = *s;
        while (*suffix) *w++ = *suffix++;
        *w = '\0';
    }

    static constexpr char hex(uint8_t n) {
        return n < 10 ? char('0' + n) : char('a' + n - 10);
    }

    static inline uint8_t mac_[6]{};
    static inline char    macStr_[13]{};
    static inline char    printerReport_[kMaxTopic]{};
    static inline char    espReport_[kMaxTopic]{};
    static inline char    espRequest_[kMaxTopic]{};
    static inline char    espSetup_[kMaxTopic]{};
};