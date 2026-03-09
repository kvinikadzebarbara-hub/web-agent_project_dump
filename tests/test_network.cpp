#include <gtest/gtest.h>
#include "network.h"

TEST(NetworkTest, Constructor) {
    NetworkClient client("http://test.com");
    // Basic test - just ensure no crash
    EXPECT_TRUE(true);
}
