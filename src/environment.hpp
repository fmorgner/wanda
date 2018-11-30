#ifndef WANDA_ENVIRONMENT_HPP
#define WANDA_ENVIRONMENT_HPP

#include <unistd.h>

#include <map>
#include <string>

namespace wanda
{

struct environment
{
    using map_type = std::map<std::string, std::string>;
    using iterator = map_type::iterator;
    using const_iterator = map_type::const_iterator;
    using reference = map_type::reference;
    using const_reference = map_type::const_reference;

    explicit environment(char const *const *env = ::environ);

    std::string &operator[](std::string const &variable);
    std::string const &operator[](std::string const &variable) const;

    iterator begin();
    const_iterator begin() const;
    const_iterator cbegin() const;

    iterator end();
    const_iterator end() const;
    const_iterator cend() const;

  private:
    map_type m_cache{};
};

} // namespace wanda

#endif