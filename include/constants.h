#pragma once

class Constants {
    public:
        static inline constexpr int CLUSTER_SIZE{ 16 };
        static inline constexpr int SHARED_DIR_PIN { 21 };
        static inline constexpr int SHARED_STEP_PIN { 22 };
        static inline constexpr int LOAD_TIMEOUT_MS { 5000 };
        static inline constexpr int ENGAGE_TIMEOUT_MS { 5000 };
        static inline constexpr int PLUS_FREQUENCY { 500 };
        static inline constexpr const char* MQTT_HOST { "127.0.0.1" };
        static inline constexpr int MQTT_PORT { 1883 };
        // MOVE TO ENV AT SOME POINT
        static inline constexpr const char* ssid { "Sozzer" };  
        static inline constexpr const char* pass { "Bluebash" };
};