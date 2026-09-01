#include "cluster.h"

bool Cluster::isAvailable() { 
    return Cluster::active == nullptr; 
};

void Cluster::begin() {
    // @TODO:
}

size_t Cluster::currReport(char* out, size_t outLen) {
    // @TODO: keep working on this - maybe have an internal function for this. instead and this just a wrapper with all items as input
    JsonDocument response;
    response["connected"] = true;
    response["idle"] = Cluster::isAvailable();
    response["configRev"] = Config::config["configRev"];
    return serializeJson(response, out, outLen);
}
