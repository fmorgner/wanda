/**
 * @file   deferred_failure.hpp
 * @author Felix Morgner (felix.morgner@gmail.com)
 * @since  1.0.0
 */

#ifndef WANDA_DEFERRED_FAILURE_HPP
#define WANDA_DEFERRED_FAILURE_HPP

#include <type_traits>

namespace
{
  /**
   * @brief A helper type to defer static_assert failures
   */
  template<typename...>
  using deferred_failure = std::false_type;
}  // namespace

#endif