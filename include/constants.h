#pragma once
#include "topicRegistry.hpp"
class Constants {
    public:
        static inline const char* MQTT_ID { TopicRegistry::macStr() };
        static inline constexpr int BUFFER_SIZE {512}; 
        
        // @TODO: Config ? 
        static inline constexpr int CLUSTER_SIZE{ 16 };
        static inline constexpr int SHARED_DIR_PIN { 21 };
        static inline constexpr int SHARED_STEP_PIN { 22 };
        static inline constexpr int LOAD_TIMEOUT_MS { 5000 };
        static inline constexpr int ENGAGE_TIMEOUT_MS { 5000 };
        static inline constexpr int PLUS_FREQUENCY_MS { 500 };
        static inline constexpr unsigned long WIFI_CONNECT_TIMEOUT_MS { 15000 };
        static inline constexpr unsigned long MQTT_RECONNECT_BACKOFF_MS { 5000 };
        static inline constexpr unsigned long PUBLISH_INTERVAL_MS { 5000 };

        // MOVE TO ENV AT SOME POINT
        static inline constexpr const char* ssid { "Sozzer" };  
        static inline constexpr const char* pass { "Bluebash" };
        static inline constexpr const char* MQTT_HOST { "192.168.1.81" };
        static inline constexpr int MQTT_PORT { 1883 };
};
