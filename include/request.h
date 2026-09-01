#pragma once
#include <ArduinoJson.h>
#include <MqttClient.h>
#include "fault.h"
class Request {
    
    public:
        enum class Command { UNKNOWN, LOAD, UNLOAD, RESOLVE, ABORT };

        struct request {
            Command cmd;
            int slot;
        };

        static bool slotRequired(Command);
        static bool isInt(JsonVariant);
        static void handleRequest(MqttClient::MessageData&);

    private: 
        static Command parse(const char* s) {
            if (!strcmp(s, "load"))    return Command::LOAD;
            if (!strcmp(s, "unload"))  return Command::UNLOAD;
            if (!strcmp(s, "resolve")) return Command::RESOLVE;
            if (!strcmp(s, "abort")) return Command::ABORT;
            return Command::UNKNOWN;   // an unknown verb is data, not a crash
        }
};
