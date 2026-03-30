#include "agent.h"
#include "logger.h"
#include "TaskExecutor.h"
#include <chrono>
#include <thread>
#include <nlohmann/json.hpp>

WebAgent::WebAgent(const Config& cfg)
    : config(cfg), network(cfg.server_url), access_code(cfg.access_code), running(false) {
    Logger::info("WebAgent created with UID: " + config.uid);
}

void WebAgent::start() {
    Logger::info("Starting WebAgent...");
    running = true;

    pollThread = std::thread(&WebAgent::pollLoop, this);

    for (int i = 0; i < 2; ++i) {
        workerThreads.emplace_back(&WebAgent::workerLoop, this);
    }

    Logger::info("WebAgent started with worker threads");
}

void WebAgent::stop() {
    Logger::info("Stopping WebAgent...");
    running = false;

    if (pollThread.joinable()) {
        pollThread.join();
    }

    for (auto& thread : workerThreads) {
        if (thread.joinable()) {
            thread.join();
        }
    }

    Logger::info("WebAgent stopped");
}

void WebAgent::pollLoop() {
    Logger::info("POLL LOOP STARTED");

    while (running) {
        Logger::info("Polling for tasks...");

        auto taskJson = network.getTask(config.uid, config.descr, access_code);

        bool hasTask = false;

        if (!taskJson.is_null() && taskJson.contains("code_responce")) {
            try {
                if (taskJson["code_responce"].is_number()) {
                    int code = taskJson["code_responce"].get<int>();
                    if (code == 1) {
                        hasTask = true;
                        Logger::info("Task available!");
                    }
                    else if (code == 0) {
                        Logger::info("No task, waiting...");
                    }
                    else if (code == -2) {
                        Logger::error("Invalid access code");
                    }
                }
                else if (taskJson["code_responce"].is_string()) {
                    std::string code = taskJson["code_responce"].get<std::string>();
                    if (code == "1") {
                        hasTask = true;
                        Logger::info("Task available!");
                    }
                    else if (code == "0") {
                        Logger::info("No task, waiting...");
                    }
                    else if (code == "-2") {
                        Logger::error("Invalid access code");
                    }
                }
            } catch (const std::exception& e) {
                Logger::error("Error parsing code_responce");
            }
        } else {
            Logger::error("Invalid response from server");
        }

        if (hasTask) {
            try {
                Task task;
                task.task_code = taskJson.value("task_code", "");
                task.options = taskJson.value("options", "");
                task.session_id = taskJson.value("session_id", "");
                task.status = taskJson.value("status", "");

                Logger::info("Received task: code=" + task.task_code + " session=" + task.session_id);

                {
                    std::lock_guard<std::mutex> lock(queueMutex);
                    taskQueue.push(task);
                    Logger::info("Task added to queue");
                }
            } catch (const std::exception& e) {
                Logger::error("Error creating task");
            }
        }

        std::this_thread::sleep_for(std::chrono::seconds(config.poll_interval_sec));
    }

    Logger::info("Poll loop stopped");
}

void WebAgent::workerLoop() {
    Logger::info("Worker thread started");

    while (running) {
        Task currentTask;
        bool hasTask = false;

        {
            std::lock_guard<std::mutex> lock(queueMutex);
            if (!taskQueue.empty()) {
                currentTask = taskQueue.front();
                taskQueue.pop();
                hasTask = true;
                Logger::info("Worker got task from queue");
            }
        }

        if (hasTask) {
            executeTask(currentTask);
        } else {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }

    Logger::info("Worker thread stopped");
}

void WebAgent::executeTask(const Task& task) {
    Logger::info("Executing task: code=" + task.task_code + " session=" + task.session_id);

    int result_code = 0;
    std::string message;
    std::vector<std::string> out_files;

    TaskExecutor executor(config.task_directory, config.result_directory, const_cast<Config*>(&config));
    result_code = executor.execute(task, message, out_files);

    Logger::info("Task execution completed");

    nlohmann::json resultData = {
        {"UID", config.uid},
        {"access_code", access_code},
        {"session_id", task.session_id},
        {"result_code", result_code},
        {"message", message},
        {"files", out_files.size()}
    };

    std::string result_json = resultData.dump();
    Logger::info("Sending result...");

    bool success = network.sendResult(config.uid, access_code, task.session_id,
                                       result_code, result_json, out_files);

    if (success) {
        Logger::info("Result sent successfully");
    } else {
        Logger::error("Failed to send result");
    }
}
