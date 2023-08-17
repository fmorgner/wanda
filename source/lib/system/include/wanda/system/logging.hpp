/**
 * @file   logging.hpp
 * @author Felix Morgner (felix.morgner@gmail.com)
 * @since  1.0.0
 */

#ifndef WANDA_SYSTEM_LOGGING_HPP
#define WANDA_SYSTEM_LOGGING_HPP

#include <spdlog/sinks/null_sink.h>
#include <spdlog/spdlog.h>

#include <memory>

namespace wanda::system
{
  /**
   * @brief A covenience alias to represent a handle for a logger
   */
  using logger_ptr = std::shared_ptr<spdlog::logger>;

  /**
   * @brief Initialize the shared logger
   *
   * @note The logger will only ever be initialized once, even if this function is called multiple times
   */
  void initialize_logger(spdlog::sink_ptr sink = std::make_shared<spdlog::sinks::null_sink_st>());

  /**
   * @brief Get the shared logger
   */
  logger_ptr get_logger();
}  // namespace wanda::system

#endif