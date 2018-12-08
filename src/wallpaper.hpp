#ifndef WANDA_WALLPAPER_HPP
#define WANDA_WALLPAPER_HPP

#include <spdlog/sinks/null_sink.h>
#include <spdlog/spdlog.h>

#include <filesystem>
#include <memory>

namespace wanda
{
  void set_wallpaper(std::filesystem::path wallpaper, std::shared_ptr<spdlog::logger> logger);
}  // namespace wanda

#endif