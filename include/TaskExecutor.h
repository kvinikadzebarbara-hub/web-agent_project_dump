#pragma once

#include <string>
#include <vector>
#include <chrono>

struct Task {
    std::string session_id;
    std::string task_code;
    std::string options;
    std::string status;
};

class Config;

class TaskExecutor {
public:
    TaskExecutor(const std::string& work_dir, const std::string& results_dir, Config* config = nullptr);

    void setConfig(Config* config);
    void setTimeout(std::chrono::seconds timeout);

    int execute(const Task& task, std::string& message, std::vector<std::string>& out_files);

private:
    std::string work_dir_;
    std::string results_dir_;
    Config* config_;
    std::chrono::seconds timeout_;

    std::vector<std::string> collectResultFiles();
    int runCommandWithTimeout(const std::string& command, std::string& output);
};
