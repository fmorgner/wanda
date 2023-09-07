#include "wandac/cli.hpp"
#include "wandac/listener.hpp"

#include <wanda/control/commander.hpp>
#include <wanda/system/environment.hpp>
#include <wanda/system/logging.hpp>
#include <wanda/system/xdg.hpp>

#include <boost/asio/io_context.hpp>
#include <spdlog/sinks/stdout_color_sinks.h>

#include <cstdlib>
#include <filesystem>
#include <iostream>

int main(int argc, char const * const * argv)
{
  auto cli = wandac::cli{};
  if (!cli.parse({argc, argv}, std::cerr))
  {
    return EXIT_FAILURE;
  }
  else if (cli.help)
  {
    cli.print_usage(std::cout);
    return EXIT_SUCCESS;
  }

  wanda::system::initialize_logger(std::make_shared<spdlog::sinks::stderr_color_sink_st>());

  auto environment = wanda::system::environment{};
  auto runtime_dir = wanda::system::xdg_path_for(wanda::system::xdg_directory::runtime_dir, environment);
  auto interface = runtime_dir / ".wanda_interface";
  auto service = boost::asio::io_context{};
  auto listener = wandac::listener{cli, service};

  auto commander = wanda::control::commander{service, interface, listener};

  wanda::system::get_logger()->info("trying to connect to wanda control interface on '{}'", interface.native());
  commander.start();

  service.run();
}
