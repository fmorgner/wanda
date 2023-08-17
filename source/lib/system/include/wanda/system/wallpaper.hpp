/**
 * @file   wallpaper.hpp
 * @author Felix Morgner (felix.morgner@gmail.com)
 * @since  1.0.0
 */

#ifndef WANDA_SYSTEM_WALLPAPER_HPP
#define WANDA_SYSTEM_WALLPAPER_HPP

#include <spdlog/sinks/null_sink.h>
#include <spdlog/spdlog.h>

#include <filesystem>
#include <memory>

namespace wanda::system
{
  /**
   * @brief Set the wallpaper to the file specified by the given path
   */
  void set_wallpaper(std::filesystem::path wallpaper);
}  // namespace wanda::system

#endif