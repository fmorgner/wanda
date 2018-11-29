#include "optional.hpp"
#include "setting.hpp"
#include "wallpaper.hpp"

#include <iostream>

namespace wanda
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

} // namespace wanda