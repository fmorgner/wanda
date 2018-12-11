#include <wanda/logging.hpp>
#include <wanda/optional.hpp>
#include <wanda/setting.hpp>
#include <wanda/wallpaper.hpp>

namespace wanda
{
  void set_wallpaper(std::filesystem::path wallpaper)
  {
    using namespace wanda::literals;
    using namespace wanda::std_ext;
    using namespace std::string_literals;

    with("org.gnome.desktop.background"_setting, [&](auto & setting) {
      with(setting["picture-uri"_key], [&](auto & value) {
        value = "file://" + wallpaper.native();
      }) ||
          [&] { get_logger()->error("invalid settings key"); };
    }) ||
        [&] { get_logger()->error("invalid setting"); };
  }

}  // namespace wanda