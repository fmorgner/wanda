/**
 * @file   message.hpp
 * @author Felix Morgner (felix.morgner@gmail.com)
 * @since  1.0.0
 */

#ifndef WANDA_PROTO_MESSAGE_HPP
#define WANDA_PROTO_MESSAGE_HPP

#include <spdlog/common.h>

#include <cstddef>
#include <istream>
#include <optional>
#include <string>
#include <string_view>

namespace wanda::proto
{
  /**
   * @brief A tag to mark messages originating from the controller
   */
  auto constexpr message_source_controller = "C";

  /**
   * @brief A tag to mark messages originating from the daemon
   */
  auto constexpr message_source_daemon = "D";

  /**
   * @brief The command of the hello message
   */
  auto constexpr message_command_hello = "HELLO";

  /**
   * @brief A control protocol message, consisting of a @p source, @p command, and @p arguments
   */
  struct message
  {
    /**
     * @brief Serialize this message into a string
     */
    explicit operator std::string() const;

    /**
     * @brief Get the size of the message as if it was serialized
     */
    std::size_t size() const;

    /**
     * @brief The source of the message
     */
    std::string source;

    /**
     * @brief The command of the message
     */
    std::string command;

    /**
     * @brief The arguments of the message command
     */
    std::optional<std::string> argument;
  };

  /**
   * @brief Deserialize a message from the given stream
   */
  std::istream & operator>>(std::istream & in, message & message);

  /**
   * @brief Serialize a message to the given stream
   */
  std::ostream & operator<<(std::ostream & out, message const & message);

}  // namespace wanda::proto

template<>
struct spdlog::fmt_lib::formatter<wanda::proto::message> : spdlog::fmt_lib::formatter<std::string>
{
  auto format(wanda::proto::message const & message, format_context & context) const -> decltype(context.out());
};

#endif