#include "command.hpp"

namespace wanda
{
  std::optional<message> command::message() const
  {
    using namespace std::string_literals;
    auto const command = [this] {
      switch (id)
      {
        case command_id::change:
          return "CHANGE"s;
        default:
          return ""s;
      }
    }();

    auto argument_string = std::string{};
    for (int index = 0ul; index < arguments.size(); ++index)
    {
      argument_string += (index) ? "," + arguments[index] : arguments[index];
    }

    if (command.empty())
    {
      return std::nullopt;
    }

    return wanda::message{"C", command, argument_string};
  }

  std::optional<command> make_command(message message)
  {
    if (message.command == "CHANGE")
    {
      return {{command_id::change}};
    }

    return std::nullopt;
  }

  command make_change_command()
  {
    return {command_id::change};
  }

}  // namespace wanda