enable_testing()

add_executable(test_web_agent
    test_config.cpp
    test_network.cpp
)

target_link_libraries(test_web_agent PRIVATE
    agent_lib
    gtest
    gtest_main
)

add_test(NAME AgentTests COMMAND test_web_agent)
