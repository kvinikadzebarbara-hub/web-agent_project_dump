#pragma once

#include <string>
#include <memory>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace cardiology {

// Макросы для удобного логирования
#define LOG_TRACE(...)    cardiology::Logger::getInstance().getLogger()->trace(__VA_ARGS__)
#define LOG_DEBUG(...)    cardiology::Logger::getInstance().getLogger()->debug(__VA_ARGS__)
#define LOG_INFO(...)     cardiology::Logger::getInstance().getLogger()->info(__VA_ARGS__)
#define LOG_WARN(...)     cardiology::Logger::getInstance().getLogger()->warn(__VA_ARGS__)
#define LOG_ERROR(...)    cardiology::Logger::getInstance().getLogger()->error(__VA_ARGS__)
#define LOG_CRITICAL(...) cardiology::Logger::getInstance().getLogger()->critical(__VA_ARGS__)

class Logger {
public:
    // Запрещаем копирование
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;
    
    // Получение экземпляра (синглтон)
    static Logger& getInstance();
    
    // Инициализация логгера
    bool initialize(const std::string& log_file, const std::string& log_level);
    
    // Получение spdlog логгера
    std::shared_ptr<spdlog::logger> getLogger() const;
    
    // Проверка инициализации
    bool isInitialized() const;

private:
    Logger() = default;
    ~Logger() = default;
    
    // Преобразование строкового уровня в spdlog уровень
    spdlog::level::level_enum parseLogLevel(const std::string& level) const;
    
    std::shared_ptr<spdlog::logger> logger_;
    bool initialized_ = false;
};

} // namespace cardiology