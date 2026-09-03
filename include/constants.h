#pragma once
#include "topicRegistry.hpp"
class Constants {
    public:
        static inline const char* MQTT_ID { TopicRegistry::macStr() };
        static inline constexpr int BUFFER_SIZE {512}; 
        
        // @TODO: Config ? 
        static inline constexpr int CLUSTER_SIZE{ 8 };
        static inline constexpr int LOAD_TIMEOUT_MS { 5000 };
        static inline constexpr int ENGAGE_TIMEOUT_MS { 5000 };
        static inline constexpr int PLUS_FREQUENCY_MS { 500 };
        static inline constexpr int STEPS_PER_TICK { 100 };
        static inline constexpr unsigned long WIFI_CONNECT_TIMEOUT_MS { 15000 };
        static inline constexpr unsigned long MQTT_RECONNECT_BACKOFF_MS { 5000 };
        static inline constexpr unsigned long PUBLISH_INTERVAL_MS { 5000 };

        // MOVE TO ENV AT SOME POINT - AND REWRITE THE GIT TREE
        static inline constexpr const char* ssid { "Sozzer" };  
        static inline constexpr const char* pass { "Bluebash" };
        static inline constexpr const char* MQTT_HOST { "192.168.1.81" };
        static inline constexpr int MQTT_PORT { 1883 };
};
