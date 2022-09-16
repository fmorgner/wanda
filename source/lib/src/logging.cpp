#include "wanda/logging.hpp"

namespace wanda
{
  std::function<void(spdlog::sink_ptr)> initializer = [](spdlog::sink_ptr sink) {
    spdlog::register_logger(std::make_shared<spdlog::logger>("wanda", sink));
    initializer = [](auto) {};
  };

  void initialize_logger(spdlog::sink_ptr sink)
  {
    initializer(sink);
  }

  logger_ptr get_logger()
  {
    initialize_logger();
    return spdlog::get("wanda");
  }

}  // namespace wanda