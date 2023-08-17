/**
 * @file   deferred_failure.hpp
 * @author Felix Morgner (felix.morgner@gmail.com)
 * @since  1.0.0
 */

#ifndef WANDA_META_DEFERRED_FAILURE_HPP
#define WANDA_META_DEFERRED_FAILURE_HPP

#include <type_traits>

namespace wanda::meta
{
  /**
   * @brief A helper type to defer static_assert failures
   */
  template<typename...>
  using deferred_failure = std::false_type;
}  // namespace wanda::meta

#endif