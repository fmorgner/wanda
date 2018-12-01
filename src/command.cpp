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

} // namespace wanda