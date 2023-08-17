/**
 * @file   keyed.hpp
 * @author Felix Morgner (felix.morgner@gmail.com)
 * @since  1.0.0
 */

#ifndef WANDA_META_KEYED_HPP
#define WANDA_META_KEYED_HPP

namespace wanda::meta
{
  /**
   * @brief A tag type to prevent construction of a type without a factory
   */
  template<typename Derived>
  struct keyed
  {
  protected:
    struct key
    {
    };

    explicit keyed(key) {}
  };

}  // namespace wanda::meta

#endif