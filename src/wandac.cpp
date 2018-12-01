#include "commander.hpp"
#include "environment.hpp"
#include "xdg.hpp"

#include <asio.hpp>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include <filesystem>
#include <memory>

int main()
{
    auto environment = wanda::environment{};
    auto log = spdlog::stdout_color_mt("wandac");
    auto interface = wanda::xdg_path_for(wanda::xdg_directory::runtime_dir, environment) / ".wanda_interface";
    auto service = asio::io_service{};
    auto commander = wanda::commander{service, interface, log};

    log->info("trying to connect to wanda control interface on '{}'", interface.native());
    commander.start();

    service.run();
}