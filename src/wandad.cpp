#include "command.hpp"
#include "control_interface.hpp"
#include "environment.hpp"
#include "filesystem.hpp"
#include "optional.hpp"
#include "setting.hpp"
#include "wallpaper.hpp"
#include "xdg.hpp"

#include <asio.hpp>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include <csignal>
#include <set>
#include <string>

namespace
{

constexpr auto image_filter = [](auto const &path) {
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
  listener(std::vector<std::filesystem::path> const &wallpapers, std::shared_ptr<spdlog::logger> logger)
      : m_wallpapers{wallpapers},
        m_logger{logger}
  {
  }

  void on_received(wanda::control_interface &interface, wanda::command command) override
  {
    switch (command.id)
    {
    case wanda::command_id::change:
    {
      auto wallpaper = wanda::random_pick(m_wallpapers);
      m_logger->info("changing wallpaper to '{}'", wallpaper.native());
      wanda::set_wallpaper(wallpaper, m_logger);
      break;
    }
    default:
      m_logger->error("received unknown command '{}'", static_cast<int>(command.id));
    }
  }

private:
  std::vector<std::filesystem::path> const &m_wallpapers;
  std::shared_ptr<spdlog::logger> m_logger;
};

} // namespace

int main()
{
  using namespace wanda::std_ext;

  auto log = spdlog::stdout_color_mt("wandad");
  log->info("wanda is starting up");

  with(wanda::scan({"/usr/share/backgrounds"}, image_filter), [&](auto const &list) {
    auto service = asio::io_service{};
    auto socket_path = wanda::xdg_path_for(wanda::xdg_directory::runtime_dir, wanda::environment{}) / ".wanda_interface";

    log->info("starting control interface on '{}'", socket_path.native());
    auto listener = ::listener{list, log};
    auto interface = wanda::make_interface(service, socket_path, listener, log);

    if (!interface)
    {
      log->error("failed to start control interface");
      return;
    }

    if (interface->start())
    {
      return;
    }

    auto signals = asio::signal_set{service, SIGINT};
    signals.async_wait([&](auto const &error, auto const signal) {
      if (!error && signal == SIGINT)
      {
        interface->shutdown();
        service.stop();
      }
    });

    auto wallpaper = wanda::random_pick(list);
    wanda::set_wallpaper(wallpaper, log);

    service.run();
  }) ||
      [&] { log->error("wallpaper directory does not exist"); };
}
