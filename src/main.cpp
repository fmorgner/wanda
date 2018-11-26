#include "setting.hpp"
#include "optional.hpp"

#include <iostream>
#include <string>

#include <typeinfo>

int main()
{
  using namespace wanda::literals;
  using namespace wanda::std_ext;
  using namespace std::string_literals;

  with("org.gnome.desktop.background"_setting, [](auto &setting) {
    with(setting["picture-uri"_key], [](auto &value) {
      std::visit([](auto &&value) {
        using ValueType = std::decay_t<decltype(value)>;
        if constexpr (std::is_same_v<ValueType, std::string>)
        {
          std::cout << value << '\n';
        }
      },
      *value);
      std::cout << (value = "file:///"s) << '\n';
    }) || [] { std::cerr << "No such key!\n"; };
  }) || [] { std::cerr << "No such setting!\n"; };
}
