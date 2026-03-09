#include "logger.h"
#include <iostream>
#include <filesystem>

namespace cardiology {

Logger& Logger::getInstance() {
    static Logger instance;
    return instance;
}

bool Logger::initialize(const std::string& log_file, const std::string& log_level) {
    try {
        // Создаем директорию для логов, если нужно
        std::filesystem::path log_path(log_file);
        if (log_path.has_parent_path()) {
            std::filesystem::create_directories(log_path.parent_path());
        }
        
        // Создаем file sink
        auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(log_file, true);
        
        // Создаем console sink
        auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        
        // Объединяем sinks
        std::vector<spdlog::sink_ptr> sinks {file_sink, console_sink};
        
        // Создаем логгер
        logger_ = std::make_shared<spdlog::logger>("agent", sinks.begin(), sinks.end());
        
        // Устанавливаем уровень логирования
        spdlog::level::level_enum level = parseLogLevel(log_level);
        logger_->set_level(level);
        logger_->flush_on(level);  // сразу сбрасывать на диск
        
        // Устанавливаем формат логов
        logger_->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] [thread %t] %v");
        
        initialized_ = true;
        LOG_INFO("Logger initialized successfully. Log file: {}, level: {}", log_file, log_level);
        
        return true;
        
    } catch (const spdlog::spdlog_ex& e) {
        std::cerr << "Logger initialization failed: " << e.what() << std::endl;
        return false;
    }
}

spdlog::level::level_enum Logger::parseLogLevel(const std::string& level) const {
    std::string lower_level;
    for (char c : level) {
        lower_level += std::tolower(c);
    }
    
    if (lower_level == "trace") return spdlog::level::trace;
    if (lower_level == "debug") return spdlog::level::debug;
    if (lower_level == "info") return spdlog::level::info;
    if (lower_level == "warn" || lower_level == "warning") return spdlog::level::warn;
    if (lower_level == "err" || lower_level == "error") return spdlog::level::err;
    if (lower_level == "critical") return spdlog::level::critical;
    
    // По умолчанию - info
    return spdlog::level::info;
}

std::shared_ptr<spdlog::logger> Logger::getLogger() const {
    return logger_;
}

bool Logger::isInitialized() const {
    return initialized_ && logger_ != nullptr;
}

} // namespace cardiology