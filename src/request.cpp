#include "request.h"
#include "cluster.h"

bool Request::slotRequired(Request::Command cmd) {
    return cmd == Command::LOAD || cmd == Command::UNLOAD;
}

bool Request::isInt(JsonVariant slot) {
    if (slot.isNull() || !slot.is<int>()) return false;
    return true;
}

void Request::handleRequest(MqttClient::MessageData& md) {
    JsonDocument request;
    const MqttClient::Message& msg = md.message;

    DeserializationError err = deserializeJson(
        request, (const char*) msg.payload, msg.payloadLen
    );
    
    if (err) {
        Fault::raise(Fault::REQ_UNPROCESSABLE);
        return;
    }
    Request::request req;
    
    // Check if a valid command and store as cmd
    const char* verb = request["cmd"];
    if (verb == nullptr) { Fault::raise(Fault::MISSING_OR_NO_CMD); return; }
    Request::Command cmd = Request::parse(verb);
    if (cmd == Request::Command::UNKNOWN) {
        Fault::raise(Fault::MISSING_OR_NO_CMD);
        return;

    }    

    req.cmd = cmd;
    
    if (Request::slotRequired(cmd) && !isInt(request["slot"])) {
        Fault::raise(Fault::MISSING_SLOT);
        return;

    }

    req.slot = request["slot"];

    // @TODO: FIGURE THIS OUT
    if (cmd !=Request::Command::ABORT && !Cluster::isAvailable()) {
        Fault::raise(Fault::BUSY);
        return;

    };

    Cluster::handleCommand(req);
}