
#include <exception>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include <nlohmann/json.hpp>

#include "utils.cpp"

struct AppErrorClientPayload {
    std::vector<std::string> errors;
    std::string errorId;
    std::optional<std::string> requestId;
    std::optional<std::string> code;
    std::optional<nlohmann::json> expose;
};

// Mirrors the TS spread-if-defined behavior: absent keys are omitted, not null.
inline void to_json(nlohmann::json& j, const AppErrorClientPayload& p) {
    j = nlohmann::json{{"errors", p.errors}, {"errorId", p.errorId}};
    if (p.requestId) j["requestId"] = *p.requestId;
    if (p.code) j["code"] = *p.code;
    if (p.expose) j["expose"] = *p.expose;
}

struct AppErrorInit {
    std::optional<int> status;
    std::optional<std::string> name;
    std::optional<std::string> code;
    std::optional<nlohmann::json> expose;
    std::optional<nlohmann::json> debug;
    std::optional<nlohmann::json> cause;
    std::optional<std::string> errorId;
};

class AppError : public std::exception {
public:
    std::string message;
    std::string name;
    int status;
    std::optional<std::string> code;
    std::optional<nlohmann::json> expose;
    std::optional<nlohmann::json> debug;
    std::optional<nlohmann::json> cause;
    std::string errorId;

    explicit AppError(std::string message, AppErrorInit init = {})
        : message(std::move(message)),
          name(init.name.value_or("AppError")),
          status(init.status.value_or(500)),
          code(std::move(init.code)),
          expose(std::move(init.expose)),
          debug(std::move(init.debug)),
          cause(std::move(init.cause)),
          errorId(init.errorId ? *std::move(init.errorId) : uuid::generate_uuid_v4()) {}

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

class HttpError : public AppError {
public:
    explicit HttpError(std::string message = "Internal Server Error", AppErrorInit init = {})
        : AppError(std::move(message), withDefaults(std::move(init))) {}

private:
    static AppErrorInit withDefaults(AppErrorInit init) {
        if (!init.name) init.name = "HTTPError";
        if (!init.status) init.status = 500;
        return init;
    }
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
