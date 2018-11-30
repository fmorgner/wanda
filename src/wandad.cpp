#include "control_interface.hpp"
#include "environment.hpp"
#include "filesystem.hpp"
#include "optional.hpp"
#include "setting.hpp"
#include "wallpaper.hpp"
#include "xdg.hpp"

#include <boost/asio.hpp>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include <csignal>
#include <iostream>
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

} // namespace

int main()
{
  using namespace wanda::std_ext;

  auto log = spdlog::stdout_color_mt("wandad");
  log->info("wanda is starting up");

  with(wanda::scan({"/usr/share/backgrounds"}, image_filter), [&](auto const &list) {
    auto wallpaper = wanda::random_pick(list);
    wanda::set_wallpaper(wallpaper);

    auto service = boost::asio::io_service{};
    auto socket_path = wanda::xdg_path_for(wanda::xdg_directory::runtime_dir, wanda::environment{}) / ".wanda_interface";

    log->info("starting control interface on '{}'", socket_path.native());
    auto interface = wanda::make_interface(service, socket_path, log);

    if(!interface)
    {
      log->error("failed to start control interface");
      return;
    }

    if (interface->start())
    {
      return;
    }

    auto signals = boost::asio::signal_set{service, SIGINT};
    signals.async_wait([&](auto const &error, auto const signal) {
      if (!error && signal == SIGINT)
      {
        interface->shutdown();
        service.stop();
      }
    });

    service.run();
  }) || [&] { log->error("wallpaper directory does not exist"); };
}
