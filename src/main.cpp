#include "agent.h"
#include "logger.h"
#include "config.h"
#include <iostream>
#include <filesystem>

int main(int argc, char* argv[]) {
    // Определяем путь к конфигурационному файлу
    std::string config_file = "config.json";
    
    // Если передан аргумент командной строки, используем его как путь к конфигу
    if (argc > 1) {
        config_file = argv[1];
    }
    
    std::cout << "Web-Agent Cardiology starting..." << std::endl;
    std::cout << "Config file: " << config_file << std::endl;
    
    // Создаем экземпляр агента
    cardiology::Agent agent;
    
    // Инициализируем агент
    if (!agent.initialize(config_file)) {
        std::cerr << "Failed to initialize agent. Exiting." << std::endl;
        return 1;
    }
    
    // Запускаем агент
    agent.run();
    
    std::cout << "Agent is running. Press Ctrl+C to stop." << std::endl;
    
    // Ждем, пока агент работает
    while (agent.isRunning()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    
    std::cout << "Agent finished." << std::endl;
    return 0;
}