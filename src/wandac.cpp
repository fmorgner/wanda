#include "commander.hpp"
#include "environment.hpp"
#include "xdg.hpp"

#include <boost/asio.hpp>

#include <filesystem>
#include <iostream>
#include <memory>

int main()
{
    auto environment = wanda::environment{};
    auto interface = wanda::xdg_path_for(wanda::xdg_directory::runtime_dir, environment) / ".wanda_interface";
    auto service = boost::asio::io_service{};

    std::clog << "[wandac::main] Initializing commander for socket '" << interface.native() << "'\n";
    auto commander = wanda::commander{service, interface};

    commander.start();

    service.run();
}