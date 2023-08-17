#ifndef WANDA_PROTO_COMMAND_HPP
#define WANDA_PROTO_COMMAND_HPP

#include "wanda/proto/message.hpp"

#include <optional>
#include <string>
#include <vector>

namespace wanda::proto
{
  /**
   * @brief An enum to describe different command IDs
   */
  enum struct command_id : char
  {
    change,  //< Change the wallpaper
  };

  /**
   * @brief A simple type to represent commands transported through the control connection
   */
  struct command
  {
    command_id const id;
    std::vector<std::string> const arguments;

    /**
     * @brief Convert the command to a message for transmission to a remote endpoint
     */
    std::optional<wanda::proto::message> message() const;
  };

  /**
   * @brief Extract a command from a message
   */
  std::optional<command> make_command(message message);

  /**
   * @brief A simple factory to create a "Change wallpaper" command
   */
  command make_change_command();

}  // namespace wanda::proto

#endif