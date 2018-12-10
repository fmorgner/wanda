/**
 * @file   environment.hpp
 * @author Felix Morgner (felix.morgner@gmail.com)
 * @since  1.0.0
 */

#ifndef WANDA_ENVIRONMENT_HPP
#define WANDA_ENVIRONMENT_HPP

#include <unistd.h>

#include <map>
#include <string>

namespace wanda
{
  /**
   * @brief A type to provide access to the runtime environment
   */
  struct environment
  {
    using map_type = std::map<std::string, std::string>;
    using iterator = map_type::iterator;
    using const_iterator = map_type::const_iterator;
    using reference = map_type::reference;
    using const_reference = map_type::const_reference;

    /**
     * @brief Construct a new environment from the given string array
     */
    explicit environment(char const * const * env = ::environ);

    /**
     * @brief Get the value of the given variable
     * 
     * @return A mutable reference to the value of the given environment variable
     */
    std::string & operator[](std::string const & variable);

    /**
     * @brief Get the value of the given variable
     * 
     * @return An immutable reference to the value of the given environment variable
     */
    std::string const & operator[](std::string const & variable) const;

    iterator begin();
    const_iterator begin() const;
    const_iterator cbegin() const;

    iterator end();
    const_iterator end() const;
    const_iterator cend() const;

  private:
    map_type m_cache{};
  };

}  // namespace wanda

#endif