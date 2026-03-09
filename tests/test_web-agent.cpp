#include <gtest/gtest.h>
#include "config.h"
#include "logger.h"
#include <filesystem>
#include <fstream>

class ConfigTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Создаем тестовый конфиг
        test_config_ = "test_config.json";
        std::ofstream file(test_config_);
        file << R"({
            "uid": "test-agent",
            "server_url": "http://test-server.com",
            "poll_interval_sec": 15,
            "log_file": "test.log",
            "log_level": "debug",
            "work_dir": "./test_work",
            "result_dir": "./test_results",
            "max_concurrent_tasks": 2
        })";
        file.close();
    }
    
    void TearDown() override {
        std::filesystem::remove(test_config_);
    }
    
    std::string test_config_;
};

TEST_F(ConfigTest, LoadValidConfig) {
    auto& config = cardiology::Config::getInstance();
    EXPECT_TRUE(config.loadFromFile(test_config_));
    EXPECT_EQ(config.getUid(), "test-agent");
    EXPECT_EQ(config.getServerUrl(), "http://test-server.com");
    EXPECT_EQ(config.getPollIntervalSec(), 15);
    EXPECT_EQ(config.getLogFile(), "test.log");
    EXPECT_EQ(config.getLogLevel(), "debug");
    EXPECT_EQ(config.getWorkDir(), "./test_work");
    EXPECT_EQ(config.getResultDir(), "./test_results");
    EXPECT_EQ(config.getMaxConcurrentTasks(), 2);
}

TEST_F(ConfigTest, MissingRequiredFields) {
    std::string invalid_config = "invalid_config.json";
    std::ofstream file(invalid_config);
    file << R"({"uid": "test-only"})";
    file.close();
    
    auto& config = cardiology::Config::getInstance();
    EXPECT_FALSE(config.loadFromFile(invalid_config));
    
    std::filesystem::remove(invalid_config);
}

TEST_F(ConfigTest, InvalidJson) {
    std::string invalid_json = "invalid.json";
    std::ofstream file(invalid_json);
    file << "This is not JSON";
    file.close();
    
    auto& config = cardiology::Config::getInstance();
    EXPECT_FALSE(config.loadFromFile(invalid_json));
    
    std::filesystem::remove(invalid_json);
}

TEST(LoggerTest, InitializeLogger) {
    auto& logger = cardiology::Logger::getInstance();
    EXPECT_TRUE(logger.initialize("test_log.log", "info"));
    EXPECT_TRUE(logger.isInitialized());
    
    // Проверяем, что файл создался
    EXPECT_TRUE(std::filesystem::exists("test_log.log"));
    
    // Очищаем
    std::filesystem::remove("test_log.log");
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}