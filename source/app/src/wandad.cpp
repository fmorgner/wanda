#include <wanda/control/interface.hpp>
#include <wanda/proto/command.hpp>
#include <wanda/std_ext/optional.hpp>
#include <wanda/system/environment.hpp>
#include <wanda/system/filesystem.hpp>
#include <wanda/system/logging.hpp>
#include <wanda/system/setting.hpp>
#include <wanda/system/wallpaper.hpp>
#include <wanda/system/xdg.hpp>

#include <boost/asio/io_context.hpp>
#include <boost/asio/signal_set.hpp>
#include <lyra/lyra.hpp>
#include <spdlog/sinks/stdout_color_sinks.h>

#include <csignal>
#include <cstdlib>
#include <iostream>
#include <set>
#include <string>

namespace
{
  constexpr auto image_filter = [](auto const & path) {
    static auto const extensions = std::set<std::filesystem::path>{
        std::filesystem::path{".jpg"},
        std::filesystem::path{".png"},
    };

    if (!std::filesystem::is_regular_file(path))
    {
      return false;
    }

    return extensions.find(path.extension()) != extensions.cend();
  };

  struct cli
  {
    std::string wallpaper_directory{};
    bool help{};

    lyra::cli_parser parser{};

    auto parse(int argc, char const * const * argv, std::ostream & error)
    {
      parser |=
          lyra::arg{wallpaper_directory, "directory"}("The wallpaper source directory").required() | lyra::help(help);

      auto result = parser.parse({argc, argv});

      if (!result)
      {
        error << "Error while processing command line arguments: " << result.message() << '\n' << parser << '\n';
        return false;
      }

      return true;
    }
  };

  struct listener : wanda::control::interface::listener
  {
    listener(std::vector<std::filesystem::path> const & wallpapers)
        : m_wallpapers{wallpapers}
    {
    }

    void on_received(wanda::control::interface & interface, wanda::proto::command command) override
    {
      switch (command.id)
      {
        case wanda::proto::command_id::change: {
          auto wallpaper = wanda::system::random_pick(m_wallpapers);
          wanda::system::get_logger()->info("changing wallpaper to '{}'", wallpaper.native());
          wanda::system::set_wallpaper(wallpaper);
          break;
        }
        default:
          wanda::system::get_logger()->error("received unknown command '{}'", static_cast<int>(command.id));
      }
    }

  private:
    std::vector<std::filesystem::path> const & m_wallpapers;
  };

}  // namespace

int main(int argc, char const * const * argv)
{
  using namespace wanda::std_ext;

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

  wanda::system::initialize_logger(std::make_shared<spdlog::sinks::stdout_color_sink_st>());
  wanda::system::get_logger()->info("wanda is starting up");

  with(wanda::system::scan({cli.wallpaper_directory}, image_filter),
       [&](auto const & list) {
         auto service = boost::asio::io_context{};
         auto socket_path =
             wanda::system::xdg_path_for(wanda::system::xdg_directory::runtime_dir, wanda::system::environment{}) /
             ".wanda_interface";

         wanda::system::get_logger()->info("starting control interface on '{}'", socket_path.native());
         auto listener = ::listener{list};
         auto interface = wanda::control::make_interface(service, socket_path, listener);

         if (!interface)
         {
           wanda::system::get_logger()->error("failed to start control interface");
           return;
         }

         if (interface->start())
         {
           return;
         }

         auto signals = boost::asio::signal_set{service, SIGINT, SIGTERM};
         signals.async_wait([&](auto const & error, auto const signal) {
           if (!error)
           {
             wanda::system::get_logger()->info("Received signal {}. terminating...", signal);
             interface->shutdown();
             service.stop();
           }
         });

         auto wallpaper = wanda::system::random_pick(list);
         wanda::system::set_wallpaper(wallpaper);

         service.run();
       }) ||
      [&] { wanda::system::get_logger()->error("wallpaper directory does not exist"); };
}
