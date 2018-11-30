#include "environment.hpp"

#include <iostream>
#include <string>

namespace wanda
{

environment::environment(char const *const *env)
{
    if (!env)
    {
        return;
    }

    std::string buffer{};
    for (; *env != nullptr; ++env)
    {
        buffer = *env;
        int split_point = buffer.find('=');
        if (split_point != std::string::npos)
        {
            m_cache[buffer.substr(0, split_point)] = buffer.substr(split_point + 1);
        }
    }
}

std::string & environment::operator[](std::string const & variable)
{
    return m_cache[variable];
}

std::string const & environment::operator[](std::string const & variable) const
{
    static std::string const empty{};
    if(auto needle = m_cache.find(variable); needle != cend())
    {
        return needle->second;
    }
    return empty;
}

environment::iterator environment::begin()
{
    return m_cache.begin();
}

environment::const_iterator environment::begin() const
{
    return m_cache.begin();
}

environment::const_iterator environment::cbegin() const
{
    return m_cache.cbegin();
}

environment::iterator environment::end()
{
    return m_cache.end();
}

environment::const_iterator environment::end() const
{
    return m_cache.end();
}

environment::const_iterator environment::cend() const
{
    return m_cache.cend();
}

} // namespace wanda