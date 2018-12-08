/**
 * @file type_wrapper.hpp
 * @author Felix Morgner (felix.morgner@gmail.com)
 * @since 1.0.0
 */

#ifndef WANDA_TYPE_WRAPPER_HPP
#define WANDA_TYPE_WRAPPER_HPP

#include <utility>

namespace wanda
{
  template<typename InnerType, typename TagType>
  struct type_wrapper
  {
    explicit type_wrapper(InnerType value)
        : m_value{std::move(value)}
    {
    }

    constexpr explicit operator InnerType const &() const { return get(); }

    constexpr InnerType const & get() const { return m_value; }

  private:
    InnerType const m_value;
  };

}  // namespace wanda

#endif