#ifndef WANDA_XDG_HPP
#define WANDA_XDG_HPP

#include "environment.hpp"

#include <cstddef>
#include <filesystem>
#include <type_traits>

namespace wanda
{
enum struct xdg_directory : std::underlying_type_t<std::byte>
{
    data_home,
    config_home,
    cache_home,
    runtime_dir,
};

std::string xdg_variable(xdg_directory directory);

std::filesystem::path xdg_path_for(xdg_directory directory, environment const &environment);
} // namespace wanda

#endif