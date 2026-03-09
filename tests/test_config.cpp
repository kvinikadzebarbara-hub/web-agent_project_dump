#include <gtest/gtest.h>
#include "config.h"
#include <fstream>
#include <nlohmann/json.hpp>

TEST(ConfigTest, LoadValidConfig) {
    std::ofstream file("test_config.json");
    file << R"({"uid": "test-001", "server_url": "http://test.com", "poll_interval_sec": 5})";
    file.close();

    Config cfg = Config::load("test_config.json");
    EXPECT_EQ(cfg.uid, "test-001");
    EXPECT_EQ(cfg.poll_interval_sec, 5);

    std::remove("test_config.json");
}

TEST(ConfigTest, LoadConfigWithWrongType) {
    std::ofstream file("test_config_wrong_type.json");
    // poll_interval_sec должен быть числом, но тут строка "five"
    file << R"({"uid": "test-002", "server_url": "http://test.com", "poll_interval_sec": "five"})";
    file.close();

    // Текущее поведение: бросается исключение nlohmann::json::type_error
    EXPECT_THROW({
        Config cfg = Config::load("test_config_wrong_type.json");
    }, nlohmann::json_abi_v3_11_2::detail::type_error);

    std::remove("test_config_wrong_type.json");
}