#ifndef WANDA_COMMAND_HPP
#define WANDA_COMMAND_HPP

#include "message.hpp"

#include <optional>
#include <string>
#include <vector>

namespace wanda
{
enum struct command_id : char
{
    change,
};

struct command
{
    command_id const id;
    std::vector<std::string> const arguments;

    std::optional<wanda::message> message() const;
};

std::optional<command> make_command(message message);

command make_change_command();

} // namespace wanda

#endif