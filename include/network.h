#pragma once
#include <string>
#include <nlohmann/json.hpp>
#include <vector>

class NetworkClient {
    std::string base_url;
public:
    NetworkClient(const std::string& url);

    bool registerAgent(const std::string& uid, const std::string& descr, std::string& out_access_code);
    nlohmann::json getTask(const std::string& uid, const std::string& descr, const std::string& access_code);
    bool sendResult(const std::string& uid, const std::string& access_code,
                    const std::string& session_id, int result_code,
                    const std::string& result_json, const std::vector<std::string>& files);
};
