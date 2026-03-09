#include "agent.h"
#include "config.h"
#include "logger.h"
#include <csignal>
#include <chrono>
#include <thread>
#include <iostream>  // <-- Добавьте этот include для std::cerr

namespace cardiology {

// Указатель на экземпляр агента для обработчика сигналов
static Agent* g_agent_instance = nullptr;

Agent::Agent() {
    g_agent_instance = this;  // <-- Сохраняем указатель на себя
    
    // Устанавливаем обработчик сигналов
    std::signal(SIGINT, Agent::signalHandler);
    std::signal(SIGTERM, Agent::signalHandler);
#ifdef _WIN32
    std::signal(SIGBREAK, Agent::signalHandler);
#else
    std::signal(SIGQUIT, Agent::signalHandler);
#endif
}

Agent::~Agent() {
    stop();
    g_agent_instance = nullptr;
}

bool Agent::initialize(const std::string& config_file) {
    // Загружаем конфигурацию
    if (!Config::getInstance().loadFromFile(config_file)) {
        std::cerr << "Failed to load configuration from: " << config_file << std::endl;
        return false;
    }
    
    // Инициализируем логгер
    if (!Logger::getInstance().initialize(
            Config::getInstance().getLogFile(),
            Config::getInstance().getLogLevel())) {
        std::cerr << "Failed to initialize logger" << std::endl;
        return false;
    }
    
    // Устанавливаем интервал опроса из конфига
    poll_interval_ = std::chrono::seconds(Config::getInstance().getPollIntervalSec());
    
    initialized_ = true;
    LOG_INFO("Agent initialized successfully. UID: {}, Server: {}", 
             Config::getInstance().getUid(),
             Config::getInstance().getServerUrl());
    
    return true;
}

void Agent::run() {
    if (!initialized_) {
        LOG_ERROR("Agent not initialized. Call initialize() first.");
        return;
    }
    
    running_ = true;
    LOG_INFO("Agent started. Poll interval: {} seconds", poll_interval_.count());
    
    // Запускаем основной цикл в отдельном потоке
    main_thread_ = std::make_unique<std::thread>(&Agent::mainLoop, this);
}

void Agent::stop() {
    if (running_) {
        LOG_INFO("Stopping agent...");
        running_ = false;
        
        if (main_thread_ && main_thread_->joinable()) {
            main_thread_->join();
        }
        
        LOG_INFO("Agent stopped");
    }
}

void Agent::mainLoop() {
    LOG_INFO("Main loop started");
    
    int iteration = 0;
    
    while (running_) {
        iteration++;
        LOG_DEBUG("Poll iteration #{}", iteration);
        
        // TODO: Здесь будет реальный опрос сервера (ЛР №3)
        // Пока просто имитируем работу
        LOG_INFO("Polling server... (iteration {})", iteration);
        
        // Спим заданный интервал
        for (int i = 0; i < poll_interval_.count() && running_; ++i) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
    
    LOG_INFO("Main loop finished");
}

bool Agent::isRunning() const {
    return running_;
}

void Agent::signalHandler(int signal) {
    // Используем глобальный указатель вместо временного объекта
    if (g_agent_instance) {
        g_agent_instance->stop();
    }
}

} // namespace cardiology