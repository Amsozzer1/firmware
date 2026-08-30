#pragma once
#include <string>

class Constants {
    public:
        static inline constexpr int CLUSTER_SIZE{ 16 };
        static inline constexpr int SHARED_DIR_PIN { 21 };
        static inline constexpr int SHARED_STEP_PIN { 22 };
        static inline constexpr int LOAD_TIMEOUT_MS { 5000 };
        static inline constexpr int ENGAGE_TIMEOUT_MS { 5000 };
        static inline constexpr int PLUS_FREQUENCY_MS { 500 };

        // @TODO: 127.0.0.1 is the ESP32 itself -- set this to the LAN IP or
        // hostname of the machine running the broker before flashing.
        static inline constexpr const char* MQTT_HOST { "192.168.1.81" };
        static inline constexpr int MQTT_PORT { 1883 };
        static inline constexpr const char* MQTT_ID {"TEST-ID"};

        static inline constexpr unsigned long WIFI_CONNECT_TIMEOUT_MS { 15000 };
        // How long to wait between broker reconnect attempts.
        static inline constexpr unsigned long MQTT_RECONNECT_BACKOFF_MS { 5000 };
        static inline constexpr unsigned long PUBLISH_INTERVAL_MS { 5000 };

        // MOVE TO ENV AT SOME POINT
        static inline constexpr const char* ssid { "Sozzer" };  
        static inline constexpr const char* pass { "Bluebash" };
};
