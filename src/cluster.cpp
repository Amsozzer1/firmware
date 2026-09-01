#include "cluster.h"
bool Cluster::isAvailable() { 
    return Cluster::active == nullptr; 
};

void Cluster::begin() {
    // @TODO: IDEK WHAT GOES HERE, IF NONE GET RID
}

size_t Cluster::currReport(char* out, size_t outLen) {
    // @TODO: keep working on this - maybe have an internal function for this. instead and this just a wrapper with all items as input
    JsonDocument response;
    response["connected"] = true;
    response["idle"] = Cluster::isAvailable();
    response["configRev"] = Config::config["configRev"];
    return serializeJson(response, out, outLen);
}
bool Cluster::slotRequired(Cluster::Command cmd) {
    return cmd == Command::LOAD || cmd == Command::UNLOAD;
}
bool Cluster::isValidSlot(JsonVariant slot) {
    if (slot.isNull()) return false;
    //@TODO complete this - check against pins
    return true;
}
void Cluster::handleRequest(MqttClient::MessageData& md) {
    JsonDocument request;
    const MqttClient::Message& msg = md.message;

    DeserializationError err = deserializeJson(
        request, (const char*) msg.payload, msg.payloadLen
    );
    
    if (err) {
        Fault::raise(Fault::REQ_UNPROCESSABLE);
        return;
    }
    Cluster::request req;
    
    // Check if a valid command and store as cmd
    const char* verb = request["cmd"];
    if (verb == nullptr) { Fault::raise(Fault::MISSING_OR_NO_CMD); return; }
    Cluster::Command cmd = Cluster::parse(verb);
    if (cmd == Cluster::Command::UNKNOWN) {
        Fault::raise(Fault::MISSING_OR_NO_CMD);
        return;

    }    

    req.cmd = cmd;
    
    if (Cluster::slotRequired(cmd) && !isValidSlot(request["slot"])) {
        Fault::raise(Fault::MISSING_SLOT);
        return;

    }

    req.slot = request["slot"];

    if (cmd !=Cluster::Command::ABORT && !Cluster::isAvailable()) {
        Fault::raise(Fault::BUSY);
        return;

    };

    Cluster::handleCommand(req);
}

void Cluster::handleCommand(Cluster::request req) {
    switch (req.cmd) {
    case Command::ABORT:   /*@TODO: HANDLE*/ return;
    case Command::LOAD:    /*@TODO: HANDLE*/ return;
    case Command::UNLOAD:  /*@TODO: HANDLE*/ return;
    case Command::RESOLVE: Fault::clean(); return;
    case Command::UNKNOWN: return;
}

     
}