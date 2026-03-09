#include "agent.h"
#include "config.h"
#include "logger.h"
#include <csignal>
#include <thread>

static WebAgent* g_agent = nullptr;
static void signalHandler(int) { if (g_agent) g_agent->stop(); }

int main() {
    try {
        Logger::init("./agent.log");
        Logger::info("=== Web Agent Started ===");

        Config cfg = Config::load("config.json");
        WebAgent agent(cfg);
        g_agent = &agent;

        std::signal(SIGINT, signalHandler);
        std::signal(SIGTERM, signalHandler);

        agent.start();

        while (g_agent) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }

    } catch (const std::exception& e) {
        Logger::error("Fatal: " + std::string(e.what()));
        return 1;
    }
    return 0;
}
