/**
 * @file   type_wrapper.hpp
 * @author Felix Morgner (felix.morgner@gmail.com)
 * @since  1.0.0
 */

#ifndef WANDA_TYPE_WRAPPER_HPP
#define WANDA_TYPE_WRAPPER_HPP

#include <utility>

namespace wanda
{
  /**
   * @brief A type to create a distinct type based on an existing type
   * 
   * @tparam InnerType The type to wrap
   * @tparam TagType   A tag type to identify the distinct type
   */
  template<typename InnerType, typename TagType>
  struct type_wrapper
  {
    /**
     * @brief Construct a new type wrapper object
     */
    explicit type_wrapper(InnerType value)
        : m_value{std::move(value)}
    {
    }

    /**
     * @brief Retrieve the wrapped value with its original type
     */
    constexpr explicit operator InnerType const &() const { return get(); }

    /**
     * @brief Retrieve the wrapped value with its original type
     */
    constexpr InnerType const & get() const { return m_value; }

  private:
    InnerType m_value;
  };

}  // namespace wanda

#endif