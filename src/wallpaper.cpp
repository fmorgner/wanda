#include "optional.hpp"
#include "setting.hpp"
#include "wallpaper.hpp"

namespace wanda
{

void set_wallpaper(std::filesystem::path wallpaper, std::shared_ptr<spdlog::logger> logger)
{
    using namespace wanda::literals;
    using namespace wanda::std_ext;
    using namespace std::string_literals;

    with("org.gnome.desktop.background"_setting, [&](auto &setting) {
        with(setting["picture-uri"_key], [&](auto &value) {
            value = "file://" + wallpaper.native();
        }) || [&] { logger->error("invalid settings key"); };
    }) || [&] { logger->error("invalid setting"); };
}

} // namespace wanda