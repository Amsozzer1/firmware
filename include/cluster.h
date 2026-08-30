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


class Cluster
{
    private:
        Module* active = nullptr;
        Module* modules[Constants::CLUSTER_SIZE] = {};


    public:
        Cluster();
        ~Cluster();
        void setConfig();
        bool isAvailable(){ return active == nullptr; };
};
