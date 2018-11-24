#include "setting.hpp"
#include "optional.hpp"

#include <iostream>
#include <string>

int main() {
  using namespace wanda::literals;
  using namespace wanda::std_ext;

  with("org.gnome.desktop.background"_setting, [](auto & setting){
    with(setting.template get<std::string>("picture-uri"_key), [](auto & value){
      std::cout << value << '\n';
    }) || []{ std::cerr << "No such key!\n"; };
  }) || []{ std::cerr << "No such setting!\n"; };
}
