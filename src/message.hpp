#ifndef WANDA_MESSAGE_HPP
#define WANDA_MESSAGE_HPP

#include <cstddef>
#include <istream>
#include <string>
#include <optional>

namespace wanda
{

struct message
{
    explicit operator std::string() const;

    std::size_t size() const;

    std::string source;
    std::string command;
    std::optional<std::string> argument;
};

std::istream & operator>>(std::istream & in, message & message);
std::ostream & operator<<(std::ostream & out, message const & message);

} // namespace wanda

#endif