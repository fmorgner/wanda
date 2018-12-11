#include <wanda/command.hpp>
#include <wanda/commander.hpp>
#include <wanda/environment.hpp>
#include <wanda/logging.hpp>
#include <wanda/xdg.hpp>

#include <asio.hpp>
#include <clara.hpp>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <memory>

struct cli
{
  std::string command{};
  bool help{};

  clara::Parser parser;

  auto parse(int argc, char const * const * argv, std::ostream & error)
  {
    parser = clara::Arg{command, "command"}("The command to send to the deamon").required() |
             clara::Help(help);

    auto result = parser.parse(clara::Args{argc, argv});

    if (!result)
    {
      error << "Error while processing command line arguments: "
            << result.errorMessage()
            << '\n'
            << parser
            << '\n';
      return false;
    }
    else if (command.empty())
    {
      error << "Missing required argument 'command'\n"
            << parser
            << '\n';
      return false;
    }

    return true;
  }
};

struct listener : wanda::commander::listener
{
  listener(::cli & cli)
      : m_cli{cli}
  {
  }

  void on_connected(wanda::commander & commander) override
  {
    if (m_cli.command == "change")
    {
      commander.send(wanda::make_change_command());
      commander.stop();
    }
  }

private:
  ::cli & m_cli;
};

int main(int argc, char const * const * argv)
{
  auto cli = ::cli{};
  if (!cli.parse(argc, argv, std::cerr))
  {
    return EXIT_FAILURE;
  }
  else if (cli.help)
  {
    std::cout << cli.parser << '\n';
    return EXIT_SUCCESS;
  }

  wanda::initialize_logger(std::make_shared<spdlog::sinks::stderr_color_sink_st>());

  auto interface = wanda::xdg_path_for(wanda::xdg_directory::runtime_dir, wanda::environment{}) / ".wanda_interface";
  auto service = asio::io_service{};
  auto listener = ::listener{cli};

  auto commander = wanda::commander{service, interface, listener};

  wanda::get_logger()->info("trying to connect to wanda control interface on '{}'", interface.native());
  commander.start();

  service.run();
}