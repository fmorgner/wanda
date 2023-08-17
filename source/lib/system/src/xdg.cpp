#include "wanda/system/xdg.hpp"

#include <unistd.h>

namespace wanda::system
{
  std::string xdg_variable(xdg_directory directory)
  {
    switch (directory)
    {
      case xdg_directory::data_home:
        return "XDG_DATA_HOME";
      case xdg_directory::config_home:
        return "XDG_CONFIG_HOME";
      case xdg_directory::cache_home:
        return "XDG_CACHE_HOME";
      case xdg_directory::runtime_dir:
        return "XDG_RUNTIME_DIR";
    }
    return "XDG_INVALID_PATH";
  }

  std::filesystem::path xdg_path_for(xdg_directory directory, environment const & environment)
  {
    if (auto path = environment[xdg_variable(directory)]; !path.empty())
    {
      return path;
    }

    auto home = std::filesystem::path{environment["HOME"]};
    switch (directory)
    {
      case xdg_directory::data_home:
        return home / ".local/share";
      case xdg_directory::config_home:
        return home / ".config";
      case xdg_directory::cache_home:
        return home / ".cache";
      case xdg_directory::runtime_dir:
        return std::filesystem::path{"/run/user"} / std::to_string(::getuid());
    }

    return "";
  }

}  // namespace wanda::system