#pragma once
#include <string>

struct Config {
    std::string uid;
    std::string descr;              // <-- ДОБАВЛЕНО (требуется по API)
    std::string server_url;
    int poll_interval_sec;
    std::string task_directory;
    std::string result_directory;
    std::string log_file;

    static Config load(const std::string& path);
};
