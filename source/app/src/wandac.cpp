#include <wanda/control/commander.hpp>
#include <wanda/proto/command.hpp>
#include <wanda/system/environment.hpp>
#include <wanda/system/logging.hpp>
#include <wanda/system/xdg.hpp>

#include <boost/asio/io_context.hpp>
#include <boost/asio/post.hpp>
#include <lyra/lyra.hpp>
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

  lyra::cli_parser parser{};

  auto parse(int argc, char const * const * argv, std::ostream & error)
  {
    parser |= lyra::arg{command, "command"}("The command to send to the deamon").required() | lyra::help(help);

    auto result = parser.parse({argc, argv});

    if (!result)
    {
      error << "Error while processing command line arguments: " << result.message() << '\n' << parser << '\n';
      return false;
    }

    return true;
  }
};

struct listener : wanda::control::commander::listener
{
  listener(::cli & cli, boost::asio::io_context & service)
      : m_cli{cli}
      , m_service{service}
  {
  }

  void on_connected(wanda::control::commander & commander) override
  {
    if (m_cli.command == "change")
    {
      commander.send(wanda::proto::make_change_command());

      post(m_service, [&] { commander.stop(); });
    }
  }

private:
  ::cli & m_cli;
  boost::asio::io_context & m_service;
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

  wanda::system::initialize_logger(std::make_shared<spdlog::sinks::stderr_color_sink_st>());

  auto interface =
      wanda::system::xdg_path_for(wanda::system::xdg_directory::runtime_dir, wanda::system::environment{}) /
      ".wanda_interface";
  auto service = boost::asio::io_context{};
  auto listener = ::listener{cli, service};

  auto commander = wanda::control::commander{service, interface, listener};

  wanda::system::get_logger()->info("trying to connect to wanda control interface on '{}'", interface.native());
  commander.start();

  service.run();
}
