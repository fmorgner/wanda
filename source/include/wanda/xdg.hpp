/**
 * @file   xdg.hpp
 * @author Felix Morgner (felix.morgner@gmail.com)
 * @since  1.0.0
 */

#ifndef WANDA_XDG_HPP
#define WANDA_XDG_HPP

#include <wanda/environment.hpp>

#include <cstddef>
#include <filesystem>
#include <type_traits>

namespace wanda
{
  /**
   * @brief An @p enum to represet the standardized XDG directories
   */
  enum struct xdg_directory : std::underlying_type_t<std::byte>
  {
    data_home,
    config_home,
    cache_home,
    runtime_dir,
  };

  /**
   * @brief Get the name of the environment variable associated with the given XDG directory
   */
  std::string xdg_variable(xdg_directory directory);

  /**
   * @brief Get the path to the given @p directory given the provided @p environment
   */
  std::filesystem::path xdg_path_for(xdg_directory directory, environment const & environment);
}  // namespace wanda

#endif