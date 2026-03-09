#pragma once

#include <string>
#include <chrono>
#include <nlohmann/json.hpp>

namespace cardiology {

class Config {
public:
    // Запрещаем копирование
    Config(const Config&) = delete;
    Config& operator=(const Config&) = delete;
    
    // Получение экземпляра (синглтон)
    static Config& getInstance();
    
    // Загрузка конфигурации из файла
    bool loadFromFile(const std::string& filename);
    
    // Геттеры для параметров конфигурации
    std::string getUid() const;
    std::string getServerUrl() const;
    int getPollIntervalSec() const;
    std::string getLogFile() const;
    std::string getLogLevel() const;
    std::string getWorkDir() const;
    std::string getResultDir() const;
    int getMaxConcurrentTasks() const;
    
    // Проверка валидности конфигурации
    bool isValid() const;

private:
    // Приватный конструктор (синглтон)
    Config() = default;
    ~Config() = default;
    
    // Парсинг JSON
    bool parseJson(const nlohmann::json& json);
    
    // Поля конфигурации
    std::string uid_;
    std::string server_url_;
    int poll_interval_sec_ = 5;  // значение по умолчанию
    std::string log_file_ = "agent.log";
    std::string log_level_ = "info";
    std::string work_dir_ = ".";
    std::string result_dir_ = "./results";
    int max_concurrent_tasks_ = 4;
    
    bool is_valid_ = false;
};

} // namespace cardiology