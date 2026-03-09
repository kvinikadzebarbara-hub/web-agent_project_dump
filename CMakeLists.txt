cmake_minimum_required(VERSION 3.15)
project(WebAgent VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Зависимости
find_package(CPR REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(spdlog REQUIRED)
find_package(GTest REQUIRED)
find_package(Threads REQUIRED)

add_executable(web_agent
    src/main.cpp
    src/config.cpp
    src/logger.cpp
    src/network.cpp
    src/agent.cpp
)

target_include_directories(web_agent PRIVATE include)

# Правильная линковка CPR
target_link_libraries(web_agent PRIVATE
    cpr::cpr           # <-- Вот правильное имя! маленькими буквами
    nlohmann_json::nlohmann_json
    spdlog::spdlog
    Threads::Threads
)

# Копируем конфиг
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/config.json
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

# Тесты
add_subdirectory(tests)