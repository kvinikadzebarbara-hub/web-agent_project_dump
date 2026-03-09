add_executable(test_web_agent
    test_web-agent.cpp
    ${CMAKE_SOURCE_DIR}/src/config.cpp
    ${CMAKE_SOURCE_DIR}/src/logger.cpp
)

target_include_directories(test_web_agent PRIVATE 
    ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(test_web_agent PRIVATE
    GTest::gtest
    GTest::gtest_main
    cpr::cpr           # <-- Тоже маленькими буквами
    nlohmann_json::nlohmann_json
    spdlog::spdlog
    Threads::Threads
)

include(GoogleTest)
gtest_discover_tests(test_web_agent)