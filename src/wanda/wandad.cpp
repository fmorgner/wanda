#include <wanda/command.hpp>
#include <wanda/control_interface.hpp>
#include <wanda/environment.hpp>
#include <wanda/filesystem.hpp>
#include <wanda/logging.hpp>
#include <wanda/optional.hpp>
#include <wanda/setting.hpp>
#include <wanda/wallpaper.hpp>
#include <wanda/xdg.hpp>

#include <asio.hpp>
#include <spdlog/sinks/stdout_color_sinks.h>

#include <csignal>
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

  struct listener : wanda::control_interface::listener
  {
    listener(std::vector<std::filesystem::path> const & wallpapers)
        : m_wallpapers{wallpapers}
    {
    }

    void on_received(wanda::control_interface & interface, wanda::command command) override
    {
      switch (command.id)
      {
        case wanda::command_id::change:
        {
          auto wallpaper = wanda::random_pick(m_wallpapers);
          wanda::get_logger()->info("changing wallpaper to '{}'", wallpaper.native());
          wanda::set_wallpaper(wallpaper);
          break;
        }
        default:
          wanda::get_logger()->error("received unknown command '{}'", static_cast<int>(command.id));
      }
    }

  private:
    std::vector<std::filesystem::path> const & m_wallpapers;
  };

}  // namespace

int main()
{
  using namespace wanda::std_ext;

  wanda::initialize_logger(std::make_shared<spdlog::sinks::stdout_color_sink_st>());
  wanda::get_logger()->info("wanda is starting up");

  with(wanda::scan({"/usr/share/backgrounds"}, image_filter), [&](auto const & list) {
    auto service = asio::io_service{};
    auto socket_path = wanda::xdg_path_for(wanda::xdg_directory::runtime_dir, wanda::environment{}) / ".wanda_interface";

    wanda::get_logger()->info("starting control interface on '{}'", socket_path.native());
    auto listener = ::listener{list};
    auto interface = wanda::make_interface(service, socket_path, listener);

    if (!interface)
    {
      wanda::get_logger()->error("failed to start control interface");
      return;
    }

    if (interface->start())
    {
      return;
    }

    auto signals = asio::signal_set{service, SIGINT};
    signals.async_wait([&](auto const & error, auto const signal) {
      if (!error && signal == SIGINT)
      {
        interface->shutdown();
        service.stop();
      }
    });

    auto wallpaper = wanda::random_pick(list);
    wanda::set_wallpaper(wallpaper);

    service.run();
  }) || [&] { wanda::get_logger()->error("wallpaper directory does not exist"); };
}
