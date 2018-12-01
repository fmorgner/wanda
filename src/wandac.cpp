#include "command.hpp"
#include "commander.hpp"
#include "environment.hpp"
#include "xdg.hpp"

#include <asio.hpp>
#include <clara.hpp>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <memory>

struct cli
{
    std::string command{};
    bool help{};

    clara::Parser parser;

    auto parse(int argc, char const *const *argv, std::ostream &error)
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
    listener(::cli &cli, std::shared_ptr<spdlog::logger> logger)
        : m_logger{logger},
          m_cli{cli}
    {
    }

    void on_connected(wanda::commander &commander) override
    {
        if (m_cli.command == "change")
        {
            commander.send({wanda::command_id::change});
        }
    }

  private:
    std::shared_ptr<spdlog::logger> m_logger;
    ::cli &m_cli;
};

int main(int argc, char const *const *argv)
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

    auto log = spdlog::stdout_color_mt("wandac");
    auto interface = wanda::xdg_path_for(wanda::xdg_directory::runtime_dir, wanda::environment{}) / ".wanda_interface";
    auto service = asio::io_service{};
    auto listener = ::listener{cli, log};

    auto commander = wanda::commander{service, interface, listener, log};

    log->info("trying to connect to wanda control interface on '{}'", interface.native());
    commander.start();

    service.run();
}