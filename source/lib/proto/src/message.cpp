#include "wanda/proto/message.hpp"

#include <fmt/format.h>

#include <ios>
#include <iterator>
#include <sstream>

namespace wanda::proto
{
  message::operator std::string() const
  {
    std::ostringstream buffer{};
    buffer << source
           << ':'
           << command;
    if (argument.has_value())
    {
      buffer << ':' << *argument;
    }
    return buffer.str();
  }

  std::size_t message::size() const
  {
    return static_cast<std::string>(*this).size();
  }

  template<typename InputIt, typename OutputIt, typename UnaryPredicate>
  OutputIt copy_until(InputIt first, InputIt last, OutputIt out, UnaryPredicate predicate)
  {
    while (first != last && !predicate(*first))
    {
      *out++ = *first++;
    }
    return out;
  }

  std::istream & operator>>(std::istream & in, message & message)
  {
    auto pos = std::istream_iterator<char>{in};
    auto end = std::istream_iterator<char>{};
    auto buffer = std::string{};

    copy_until(pos, end, std::back_inserter(buffer), [](auto const & c) { return c == ':'; });
    if (in.eof() || buffer.size() != 1)
    {
      in.setstate(std::ios_base::failbit);
      return in;
    }
    message.source = buffer;

    buffer.clear();
    copy_until(++pos, end, std::back_inserter(buffer), [](auto const & c) { return c == ':'; });
    if (in.eof())
    {
      in.setstate(std::ios_base::failbit);
    }
    message.command = buffer;

    buffer.clear();
    copy(++pos, end, std::back_inserter(buffer));
    if (buffer.size())
    {
      message.argument = std::optional{std::move(buffer)};
    }

    in.clear(in.rdstate() ^ std::ios_base::failbit);
    return in;
  }

  std::ostream & operator<<(std::ostream & out, message const & message)
  {
    return out << static_cast<std::string>(message);
  }

}  // namespace wanda::proto

auto fmt::formatter<wanda::proto::message>::format(wanda::proto::message const & message, format_context & context) const -> decltype(context.out())
{
  return formatter<std::string>::format(static_cast<std::string>(message), context);
}
