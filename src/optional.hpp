/**
 * @file   optional.hpp
 * @author Felix Morgner (felix.morgner@gmail.com)
 * @since  1.0.0
 */

#ifndef WANDA_OPTIONAL_HPP
#define WANDA_OPTIONAL_HPP

#include <optional>

namespace wanda::std_ext
{
  /**
   * @brief A type to represent a computation that could fail
   */
  struct failable
  {
    /**
     * @brief A factory to create a successful computation
     */
    constexpr static auto success() { return failable{false}; }

    /**
     * @brief A factory to create a failed computation
     */
    constexpr static auto failure() { return failable{true}; }

    /**
     * @brief Execute the given handler if the computation failed
     */
    template<typename Handler>
    constexpr void operator||(Handler handler) const
    {
      if (m_failed)
      {
        handler();
      }
    }

  private:
    constexpr explicit failable(bool failed)
        : m_failed{failed} {};
    bool const m_failed;
  };

  /**
   * @brief Unwrap the given optional object, if present, and pass it to the handler
   * 
   * @return A successful computation iff. the object was present, a failed computation otherwise.
   */
  template<typename ObjectType, typename HandlerType>
  auto with(std::optional<ObjectType> && object, HandlerType handler)
  {
    if (object)
    {
      handler(object.value());
      return failable::success();
    }
    return failable::failure();
  }

}  // namespace wanda::std_ext

#endif