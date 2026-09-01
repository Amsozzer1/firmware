#include "fault.h"
#include <Arduino.h>
#include "cluster.h"
#include "config.h"

void Fault::clean() {
    Cluster::error = Fault::NO_FAULT;
}

void Fault::raise(int code) {
    if (Cluster::error == Fault::NO_FAULT) Cluster::error = code;
    if (!Config::configured) Serial.printf("Error %i and brain not reachable", code);
}
