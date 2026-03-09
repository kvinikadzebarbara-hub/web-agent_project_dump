#include "config.h"
#include <fstream>
#include <iostream>
#include <filesystem>
#include <nlohmann/json.hpp>

namespace cardiology {

using json = nlohmann::json;

Config& Config::getInstance() {
    static Config instance;
    return instance;
}

bool Config::loadFromFile(const std::string& filename) {
    try {
        // Проверяем существование файла
        if (!std::filesystem::exists(filename)) {
            std::cerr << "Config file not found: " << filename << std::endl;
            return false;
        }
        
        // Читаем файл
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Cannot open config file: " << filename << std::endl;
            return false;
        }
        
        // Парсим JSON
        json config_json;
        file >> config_json;
        
        // Парсим содержимое
        bool parse_result = parseJson(config_json);
        is_valid_ = parse_result;
        
        return parse_result;
        
    } catch (const json::parse_error& e) {
        std::cerr << "JSON parse error: " << e.what() << std::endl;
        return false;
    } catch (const std::exception& e) {
        std::cerr << "Config load error: " << e.what() << std::endl;
        return false;
    }
}

bool Config::parseJson(const json& json) {
    try {
        // Обязательные поля
        if (!json.contains("uid") || !json.contains("server_url")) {
            std::cerr << "Missing required fields: uid and server_url are required" << std::endl;
            return false;
        }
        
        uid_ = json["uid"].get<std::string>();
        server_url_ = json["server_url"].get<std::string>();
        
        // Необязательные поля со значениями по умолчанию
        if (json.contains("poll_interval_sec")) {
            poll_interval_sec_ = json["poll_interval_sec"].get<int>();
        }
        
        if (json.contains("log_file")) {
            log_file_ = json["log_file"].get<std::string>();
        }
        
        if (json.contains("log_level")) {
            log_level_ = json["log_level"].get<std::string>();
        }
        
        if (json.contains("work_dir")) {
            work_dir_ = json["work_dir"].get<std::string>();
        }
        
        if (json.contains("result_dir")) {
            result_dir_ = json["result_dir"].get<std::string>();
        }
        
        if (json.contains("max_concurrent_tasks")) {
            max_concurrent_tasks_ = json["max_concurrent_tasks"].get<int>();
        }
        
        // Валидация значений
        if (uid_.empty() || server_url_.empty()) {
            std::cerr << "uid and server_url cannot be empty" << std::endl;
            return false;
        }
        
        if (poll_interval_sec_ <= 0) {
            poll_interval_sec_ = 5;  // восстанавливаем значение по умолчанию
        }
        
        if (max_concurrent_tasks_ <= 0) {
            max_concurrent_tasks_ = 4;
        }
        
        return true;
        
    } catch (const std::exception& e) {
        std::cerr << "Error parsing config: " << e.what() << std::endl;
        return false;
    }
}

// Геттеры
std::string Config::getUid() const { return uid_; }
std::string Config::getServerUrl() const { return server_url_; }
int Config::getPollIntervalSec() const { return poll_interval_sec_; }
std::string Config::getLogFile() const { return log_file_; }
std::string Config::getLogLevel() const { return log_level_; }
std::string Config::getWorkDir() const { return work_dir_; }
std::string Config::getResultDir() const { return result_dir_; }
int Config::getMaxConcurrentTasks() const { return max_concurrent_tasks_; }
bool Config::isValid() const { return is_valid_; }

} // namespace cardiology