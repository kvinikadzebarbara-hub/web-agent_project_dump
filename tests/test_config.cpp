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

TEST(ConfigTest, LoadInvalidPath) {
    EXPECT_THROW(Config::load("nonexistent.json"), std::runtime_error);
}
