cmake_minimum_required(VERSION 3.15)
project(WebAgent VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- FetchContent для зависимостей ---
include(FetchContent)

# CPR
FetchContent_Declare(
    cpr
    GIT_REPOSITORY https://github.com/libcpr/cpr.git
    GIT_TAG 1.10.5
)
FetchContent_MakeAvailable(cpr)

# nlohmann/json
FetchContent_Declare(
    json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.2
)
FetchContent_MakeAvailable(json)

# spdlog
FetchContent_Declare(
    spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG v1.12.0
)
FetchContent_MakeAvailable(spdlog)

# GTest
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.14.0
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Системные библиотеки
find_package(Threads REQUIRED)

# --- БИБЛИОТЕКА (основная логика) ---
add_library(agent_lib STATIC
    src/config.cpp
    src/logger.cpp
    src/network.cpp
    src/agent.cpp
)

target_include_directories(agent_lib PUBLIC include)

target_link_libraries(agent_lib PUBLIC
    cpr::cpr
    nlohmann_json::nlohmann_json
    spdlog::spdlog
    Threads::Threads
)

# --- ИСПОЛНЯЕМЫЙ ФАЙЛ ---
add_executable(web_agent src/main.cpp)
target_link_libraries(web_agent PRIVATE agent_lib)

# Копирование конфига
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/config.json
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

# --- ТЕСТЫ ---
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/tests/CMakeLists.txt")
    add_subdirectory(tests)
endif()
