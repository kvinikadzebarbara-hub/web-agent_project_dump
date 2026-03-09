#include "logger.h"
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <memory>

static std::shared_ptr<spdlog::logger> g_logger;

void Logger::init(const std::string& filename) {
    g_logger = spdlog::basic_logger_mt("agent_logger", filename);
    spdlog::set_default_logger(g_logger);
    spdlog::set_pattern("[%Y-%m-%d %H:%M:%S] [%l] %v");
}

void Logger::info(const std::string& msg) { spdlog::info(msg); }
void Logger::error(const std::string& msg) { spdlog::error(msg); }
void Logger::warn(const std::string& msg) { spdlog::warn(msg); }
void Logger::debug(const std::string& msg) { spdlog::debug(msg); }
