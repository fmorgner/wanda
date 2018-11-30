#ifndef WANDA_MESSAGE_HPP
#define WANDA_MESSAGE_HPP

#include <cstddef>
#include <istream>
#include <string>
#include <optional>

namespace wanda
{

inline namespace v1
{
auto constexpr message_argument_hello = "1.0.0";
}

auto constexpr message_source_controller = "C";
auto constexpr message_source_daemon = "D";

auto constexpr message_command_hello = "HELLO";

struct message
{
    explicit operator std::string() const;

    std::size_t size() const;

    std::string source;
    std::string command;
    std::optional<std::string> argument;
};

std::istream &operator>>(std::istream &in, message &message);
std::ostream &operator<<(std::ostream &out, message const &message);

} // namespace wanda

#endif