#pragma once
#include <exception>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include <ArduinoJson.h>
#include "utils.hpp"

// @TODO: Maybe replace this with logger for MQTT-broker over on Brain?
using JsonText = std::string;

struct AppErrorClientPayload {
    std::vector<std::string> errors;
    std::string errorId;
    std::optional<std::string> requestId;
    std::optional<std::string> code;
    std::optional<JsonText> expose;
};

inline void toJson(JsonDocument& doc, const AppErrorClientPayload& p) {
    JsonArray errs = doc["errors"].to<JsonArray>();
    for (const auto& e : p.errors) errs.add(e);
    doc["errorId"] = p.errorId;
    if (p.requestId) doc["requestId"] = *p.requestId;
    if (p.code)      doc["code"]      = *p.code;
    if (p.expose)    doc["expose"]    = serialized(*p.expose);
}


struct AppErrorInit {
    std::optional<int> status;
    std::optional<std::string> name;
    std::optional<std::string> code;
    std::optional<JsonText> expose;
    std::optional<JsonText> debug;
    std::optional<JsonText> cause;
    std::optional<std::string> errorId;
};

class AppError : public std::exception {
public:
    std::string message;
    std::string name;
    int status;
    std::optional<std::string> code;
    std::optional<JsonText> expose;
    std::optional<JsonText> debug;
    std::optional<JsonText> cause;
    std::string errorId;

    explicit AppError(std::string message, AppErrorInit init = {})
        : message(std::move(message)),
          name(init.name.value_or("AppError")),
          status(init.status.value_or(500)),
          code(std::move(init.code)),
          expose(std::move(init.expose)),
          debug(std::move(init.debug)),
          cause(std::move(init.cause)),
          errorId(init.errorId ? *std::move(init.errorId) : generateUuidV4()) {}

    [[nodiscard]] AppErrorClientPayload toClientPayload(
        std::optional<std::string> requestId = std::nullopt) const {
        AppErrorClientPayload payload;
        payload.errors = {message};
        payload.errorId = errorId;
        payload.requestId = std::move(requestId);
        payload.code = code;
        payload.expose = expose;
        return payload;
    }

    const char* what() const noexcept override { return message.c_str(); }
};


class ModuleError : public AppError {
    public:
    explicit ModuleError(std::string message = "Internal Server Error", AppErrorInit init = {})
        : AppError(std::move(message), withDefaults(std::move(init))) {}
    private:
    static AppErrorInit withDefaults(AppErrorInit init) {
        if (!init.name) init.name = "ModuleError";
        if (!init.status) init.status = 500;
        return init;
    };
};
class TimeAnamoly: public ModuleError {
    public:
        explicit TimeAnamoly(
            std::string message = "Filament not engaged with Module Filament Sensor",
            AppErrorInit init = {}
        ): ModuleError(std::move(message), withDefaults(init)){}
    private:
        static AppErrorInit withDefaults(AppErrorInit init) {
            if (!init.name) init.name = "ModuleError";
            if (!init.status) init.status = 500;
            return init;
        };
};
