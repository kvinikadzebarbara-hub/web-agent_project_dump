#pragma once

#include <string>
#include <atomic>
#include <memory>
#include <thread>
#include <chrono>

namespace cardiology {

class Agent {
public:
    Agent();
    ~Agent();
    
    // Инициализация агента
    bool initialize(const std::string& config_file);
    
    // Запуск агента
    void run();
    
    // Остановка агента
    void stop();
    
    // Проверка состояния
    bool isRunning() const;

private:
    // Основной цикл опроса сервера
    void mainLoop();
    
    // Обработчик сигналов (статический)
    static void signalHandler(int signal);
    
    std::atomic<bool> running_{false};
    std::atomic<bool> initialized_{false};
    std::unique_ptr<std::thread> main_thread_;
    
    // Интервал опроса (будет загружен из конфига)
    std::chrono::seconds poll_interval_{5};
};

} // namespace cardiology