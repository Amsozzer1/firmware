#pragma once
#include <stdint.h>
#include "constants.h"
class Module {
    private:
        enum class Job { NONE, LOAD, UNLOAD };
        bool sensedFilamentInPrinter();
        bool sensedFilament();
        // A move is two legs: cross the near sensor, then run to the far one.
        bool crossed();
        bool arrived();
        void begin(Job, int dir);
        void pulse();
        int enablePin;
        int sensorPin;
        Job job;
        bool engaged;
        uint32_t start;

    public:
        Module(int, int);
        ~Module();
        void load();
        void unLoad();
        void stop();
        bool tick();
};
