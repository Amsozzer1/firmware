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
#include <ArduinoJson.h>
#include "request.h"
class Cluster
{
    static void handleCommand(Request);
    private:
        static inline Module* active {nullptr};
        static inline Module* modules[Constants::CLUSTER_SIZE] {};

    public:
        static inline int error = 0; 
        static bool isAvailable();
        static size_t currReport (char* out, size_t outLen);
        
};
