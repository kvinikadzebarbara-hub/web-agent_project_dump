#pragma once
#include "config.h"
#include "network.h"
#include <thread>
#include <atomic>
#include <queue>
#include <mutex>

struct Task {
    std::string session_id;
    std::string task_code;
    std::string options;
};

class WebAgent {
    Config config;
    NetworkClient network;
    std::string access_code;
    std::atomic<bool> running;
    std::thread pollThread;
    std::queue<Task> taskQueue;
    std::mutex queueMutex;
    std::vector<std::thread> workerThreads;

public:
    WebAgent(const Config& cfg);
    void start();
    void stop();

private:
    void pollLoop();
    void workerLoop();
    void executeTask(const Task& task);
};
