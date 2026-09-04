#include "cluster.h"
#include "config.h"
#include "fault.h"

bool Cluster::isAvailable() { 
    return Cluster::active == nullptr; 
};

size_t Cluster::currReport(char* out, size_t outLen) {
    JsonDocument response;
    response["connected"] = true;
    response["idle"] = Cluster::isAvailable();
    response["configRev"] = Config::config["configRev"];
    if (Cluster::error != Fault::NO_FAULT) response["error"] = Cluster::error;
    return serializeJson(response, out, outLen);
}

bool Cluster::sensedFilamentInPrinter() {
    return Cluster::filamentInPrinter;
}

void Cluster::setPrinterReport(const char* json, size_t len) {
    JsonDocument report;

    DeserializationError err = deserializeJson(report, json, len);
    if (err) {
        // @TODO: add a better error code here
        Fault::raise(Fault::REQ_UNPROCESSABLE);
        return;
    }
    JsonVariant sensed = report["sensed"];
    if (!sensed.is<bool>()) {
        // @TODO: add a better error code here
        Fault::raise(Fault::REQ_UNPROCESSABLE);
        return;
    }

    Cluster::filamentInPrinter = sensed;
}

bool Cluster::tick() {
    if (Cluster::active == nullptr) return false;
    if (!Cluster::active->tick()) Cluster::active = nullptr;
    return true;
}

void Cluster::handleCommand(Request::request req) {
    switch (req.cmd) {
        case Request::Command::ABORT:   Cluster::abort(); return;
        case Request::Command::LOAD:    Cluster::load(req); return;
        case Request::Command::UNLOAD: Cluster::unLoad(req); return;
        case Request::Command::RESOLVE: Fault::clean(); return;
        case Request::Command::UNKNOWN: return;
    }
}

Module* Cluster::pinToModule(int slot) {
    return Cluster::modules[slot];
}

void Cluster::abort() {
    if (Cluster::active == nullptr) return;
    Cluster::active->stop();
    Cluster::active = nullptr;
}

void Cluster::load(Request::request req) {
    Module* module = Cluster::pinToModule(req.slot);
    if (module == nullptr) { Fault::raise(Fault::MISSING_SLOT); return; }
    Cluster::active = module;
    module->load();
}

void Cluster::unLoad(Request::request req) {
    Module* module = Cluster::pinToModule(req.slot);
    if (module == nullptr) { Fault::raise(Fault::MISSING_SLOT); return; }
    Cluster::active = module;
    module->unLoad();
}