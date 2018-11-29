#include "filesystem.hpp"
#include "setting.hpp"
#include "optional.hpp"
#include "control_interface.hpp"

#include <boost/asio.hpp>

#include <csignal>
#include <iostream>
#include <set>
#include <string>

namespace
{

void set_wallpaper(std::filesystem::path wallpaper)
{
  using namespace wanda::literals;
  using namespace wanda::std_ext;
  using namespace std::string_literals;

  with("org.gnome.desktop.background"_setting, [&](auto &setting) {
    with(setting["picture-uri"_key], [&](auto &value) {
      value = "file://" + wallpaper.native();
    }) || [] { std::cerr << "No such key!\n"; };
  }) || [] { std::cerr << "No such setting!\n"; };
}

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

  with(wanda::scan({"/usr/share/backgrounds"}, image_filter), [](auto const &list) {
    auto wallpaper = wanda::random_pick(list);
    std::cout << "changing wallpaper to " << wallpaper << '\n';
    set_wallpaper(wallpaper);

    auto service = boost::asio::io_service{};
    auto interface = wanda::make_interface(service, ".wanda_interface");
    auto status = interface->start();

    if(status)
    {
      return;
    }

    auto signals = boost::asio::signal_set{service, SIGINT};
    signals.async_wait([&](auto const & error, auto const signal){
      if(!error && signal == SIGINT)
      {
        service.stop();
      }
    });

    service.run();
  }) || [] { std::cerr << "Directory does not exist\n"; };
}
