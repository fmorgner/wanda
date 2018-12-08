/**
 * @file optional.hpp
 * @author Felix Morgner (felix.morgner@gmail.com)
 * @since 1.0.0
 */

#ifndef WANDA_OPTIONAL_HPP
#define WANDA_OPTIONAL_HPP

#include <optional>

namespace wanda::std_ext
{
  struct failable
  {
    constexpr static auto success() { return failable{false}; }
    constexpr static auto failure() { return failable{true}; }

    template<typename Handler>
    constexpr auto operator||(Handler handler) const
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