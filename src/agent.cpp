#include "agent.h"
#include "logger.h"
#include <chrono>
#include <nlohmann/json.hpp>

WebAgent::WebAgent(const Config& cfg)
    : config(cfg), network(cfg.server_url), running(false) {}

void WebAgent::start() {
    running = true;
    Logger::info("Agent starting...");

    // Регистрация
    while (running && !network.registerAgent(config.uid, config.descr, access_code)) {
        Logger::warn("Registration failed, retrying in 10s...");
        std::this_thread::sleep_for(std::chrono::seconds(10));
    }
    if (!running) return;

    // Рабочие потоки (2 потока для параллельной работы)
    for (int i = 0; i < 2; ++i) {
        workerThreads.emplace_back(&WebAgent::workerLoop, this);
    }

    // Поток опроса
    pollThread = std::thread(&WebAgent::pollLoop, this);
}

void WebAgent::stop() {
    running = false;
    if (pollThread.joinable()) pollThread.join();
    for (auto& t : workerThreads) {
        if (t.joinable()) t.join();
    }
    Logger::info("Agent stopped");
}

void WebAgent::pollLoop() {
    while (running) {
        auto taskJson = network.getTask(config.uid, config.descr, access_code);

        bool hasTask = false;

        if (!taskJson.is_null() && taskJson.contains("code_responce")) {
    try {
        // code_responce может быть числом ИЛИ строкой
        if (taskJson["code_responce"].is_number()) {
            int code = taskJson["code_responce"].get<int>();
            if (code == 1) hasTask = true;
            else if (code == 0) Logger::debug("No task, waiting...");
            else if (code == -2) Logger::error("Invalid access code");
        }
        else if (taskJson["code_responce"].is_string()) {
            std::string code = taskJson["code_responce"].get<std::string>();
            if (code == "1") hasTask = true;
            else if (code == "0") Logger::debug("No task, waiting...");
            else if (code == "-2") Logger::error("Invalid access code");
        }
    } catch (const std::exception& e) {
        Logger::error("Error parsing code_responce: " + std::string(e.what()));
    }
}

        if (hasTask) {
            Task t;
            t.session_id = taskJson.value("session_id", "");
            t.task_code = taskJson.value("task_code", "");


            {
                std::lock_guard<std::mutex> lock(queueMutex);
                taskQueue.push(t);
            }
            Logger::info("Task received: " + t.task_code + " Session: " + t.session_id);
        } else {
            std::this_thread::sleep_for(std::chrono::seconds(config.poll_interval_sec));
        }
    }
}

void WebAgent::workerLoop() {
    while (running) {
        Task currentTask;
        bool hasTask = false;

        {
            std::lock_guard<std::mutex> lock(queueMutex);
            if (!taskQueue.empty()) {
                currentTask = taskQueue.front();
                taskQueue.pop();
                hasTask = true;
            }
        }

        if (hasTask) {
            executeTask(currentTask);
        } else {
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
    }
}

void WebAgent::executeTask(const Task& task) {
    Logger::info("Executing task: " + task.task_code + " Session: " + task.session_id);

    int result_code = 0;

    // Формируем result JSON по API (должен содержать UID, access_code, message, files, session_id)
    nlohmann::json resultData = {
        {"UID", config.uid},
        {"access_code", access_code},
        {"message", "Task completed successfully"},
        {"files", 0},
        {"session_id", task.session_id}
    };
    std::string result_json = resultData.dump();

    std::vector<std::string> files = {};

    network.sendResult(config.uid, access_code, task.session_id, result_code, result_json, files);
}
