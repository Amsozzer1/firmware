#pragma once
/*
    A Cluster is a collection of at-most 16 Modules
    Components:
        - TMC2209 - Driver
        - NEMA 17 - Stepper Motor
        - Filament Sensor - undecided

*/
#include "module.h"
#include "constants.h"
#include "config.h"
#include <ArduinoJson.h>
#include "fault.h"

class Cluster
{
    enum class Command { UNKNOWN, LOAD, UNLOAD, RESOLVE, ABORT };

    static Command parse(const char* s) {
        if (!strcmp(s, "load"))    return Command::LOAD;
        if (!strcmp(s, "unload"))  return Command::UNLOAD;
        if (!strcmp(s, "resolve")) return Command::RESOLVE;
        if (!strcmp(s, "abort")) return Command::ABORT;
        return Command::UNKNOWN;   // an unknown verb is data, not a crash
    }
    struct request {
        Command cmd;
        int slot; // @TODO: this could also be null 
    };

    static bool slotRequired(Command);
    static bool isValidSlot(JsonVariant);
    static void handleCommand(request);
    private:
        static inline Module* active {nullptr};
        static inline Module* modules[Constants::CLUSTER_SIZE] {};

    public:
        static inline int error = 0; 
        static bool isAvailable();
        static size_t currReport (char* out, size_t outLen);
        static void handleRequest(MqttClient::MessageData&);
};
