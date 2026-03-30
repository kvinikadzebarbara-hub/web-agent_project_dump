#include "config.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <stdexcept>

Config Config::load(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open config file: " + path);
    }

    nlohmann::json j;
    file >> j;

    Config cfg;
    cfg.uid = j.value("uid", "unknown");
    cfg.descr = j.value("descr", "web-agent");
    cfg.server_url = j.value("server_url", "");
    cfg.poll_interval_sec = j.value("poll_interval_sec", 10);
    cfg.task_directory = j.value("task_directory", "./tasks");
    cfg.result_directory = j.value("result_directory", "./results");
    cfg.log_file = j.value("log_file", "./agent.log");
    cfg.access_code = j.value("access_code", "");                    //access code
    return cfg;
}
