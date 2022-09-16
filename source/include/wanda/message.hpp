/**
 * @file   message.hpp
 * @author Felix Morgner (felix.morgner@gmail.com)
 * @since  1.0.0
 */

#ifndef WANDA_MESSAGE_HPP
#define WANDA_MESSAGE_HPP

#include <cstddef>
#include <istream>
#include <optional>
#include <string>

namespace wanda
{
  inline namespace v1
  {
    /**
     * @brief The version argument for the hello message reflecting the current version
     */
    auto constexpr message_argument_hello = "1.0.0";
  }  // namespace v1

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

}  // namespace wanda

#endif