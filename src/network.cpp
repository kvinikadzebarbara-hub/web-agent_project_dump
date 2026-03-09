#include "network.h"
#include "logger.h"
#include <cpr/cpr.h>

NetworkClient::NetworkClient(const std::string& url) : base_url(url) {}

bool NetworkClient::registerAgent(const std::string& uid, const std::string& descr, std::string& out_access_code) {
    nlohmann::json payload = {{"UID", uid}, {"descr", descr}};

    auto r = cpr::Post(
        cpr::Url{base_url + "/wa_reg/"},
        cpr::Body{payload.dump()},
        cpr::Header{{"Content-Type", "application/json"}}
    );

    if (r.status_code == 200) {
        try {
            auto res = nlohmann::json::parse(r.text);
            if (res.contains("code_responce")) {
                std::string code = res["code_responce"].get<std::string>();
                if (code == "0") {
                    out_access_code = res.value("access_code", "");
                    Logger::info("Registration successful");
                    return true;
                } else if (code == "-3") {
                    Logger::error("Agent already registered: " + res.value("msg", ""));
                    out_access_code = res.value("access_code", "");
                    return true;
                }
                Logger::error("Registration failed: " + res.value("msg", "Unknown"));
                return false;
            }
            Logger::error("No code_responce in response");
        } catch (const std::exception& e) {
            Logger::error("JSON parse error on registration: " + std::string(e.what()));
        }
    }
    return false;
}

nlohmann::json NetworkClient::getTask(const std::string& uid, const std::string& descr, const std::string& access_code) {
    nlohmann::json payload = {{"UID", uid}, {"descr", descr}, {"access_code", access_code}};

    auto r = cpr::Post(
        cpr::Url{base_url + "/wa_task/"},
        cpr::Body{payload.dump()},
        cpr::Header{{"Content-Type", "application/json"}}
    );

    if (r.status_code == 200) {
        try {
            return nlohmann::json::parse(r.text);
        } catch (const std::exception& e) {
            Logger::error("JSON parse error on getTask: " + std::string(e.what()));
        }
    }
    return nullptr;
}

bool NetworkClient::sendResult(const std::string& uid, const std::string& access_code,
                               const std::string& session_id, int result_code,
                               const std::string& result_json, const std::vector<std::string>& files) {
    // Формируем multipart/form-data по API
    cpr::Multipart multipart{
        {"result_code", std::to_string(result_code)},
        {"result", result_json},
        {"UID", uid},
        {"access_code", access_code},
        {"session_id", session_id}
    };

    // Добавляем файлы (file1, file2, file3...)
    for (size_t i = 0; i < files.size(); ++i) {
        multipart.parts.push_back({"file" + std::to_string(i+1), cpr::File{files[i]}});
    }

    auto r = cpr::Post(cpr::Url{base_url + "/wa_result/"}, multipart);

    if (r.status_code == 200) {
        try {
            auto res = nlohmann::json::parse(r.text);
            if (res.contains("code_responce")) {
                std::string code = res["code_responce"].get<std::string>();
                if (code == "0") {
                    Logger::info("Result sent successfully");
                    return true;
                }
                Logger::error("Result rejected: " + res.value("msg", ""));
                return false;
            }
        } catch (const std::exception& e) {
            Logger::error("JSON parse error on sendResult: " + std::string(e.what()));
        }
    }
    Logger::error("Failed to send result (HTTP " + std::to_string(r.status_code) + ")");
    return false;
}
