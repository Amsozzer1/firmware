#pragma once
#include "constants.h"
class Module {
    private:
        int enable;
        int id;
        bool sensedFilamentInPrinter();
        bool sensedFilament();
    
    public:
        Module(int);
        void load();
        void unLoad();
};
