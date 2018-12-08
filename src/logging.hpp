#ifndef WANDA_LOGGING_HPP
#define WANDA_LOGGING_HPP

#include <spdlog/sinks/null_sink.h>
#include <spdlog/spdlog.h>

#include <memory>

namespace wanda
{
  using logger_ptr = std::shared_ptr<spdlog::logger>;

  void initialize_logger(spdlog::sink_ptr sink = std::make_shared<spdlog::sinks::null_sink_st>());

  logger_ptr get_logger();
}  // namespace wanda

#endif