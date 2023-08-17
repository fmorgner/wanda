/**
 * @file   expected.hpp
 * @author Felix Morgner (felix.morgner@gmail.com)
 * @since  1.0.0
 */

#ifndef WANDA_STD_EXT_EXPECTED_HPP
#define WANDA_STD_EXT_EXPECTED_HPP

#include <initializer_list>
#include <type_traits>
#include <utility>

namespace wanda::std_ext
{
  /**
   * @brief A type to represent the error case of a computation based on #wanda::expected
   */
  template<typename ErrorType>
  struct unexpected
  {
    static_assert(!std::is_same_v<ErrorType, void>, "ErrorType can not be 'void'!");
    static_assert(!std::is_array_v<ErrorType>, "ErrorType can not be an array type!");

    /**
     * @brief Copy construct a new @p unexpected from another @p unexpected
     */
    constexpr unexpected(unexpected const &) = default;

    /**
     * @brief Move construct a new @p unexpected from another @p unexpected
     */
    constexpr unexpected(unexpected &&) = default;

    /**
     * @brief Construct a new @p unexpected by direct initializing the error object from @p args
     */
    template<typename... Args>
    constexpr explicit unexpected(std::in_place_t, Args &&... args)
        : m_error(std::forward<Args>(args)...)
    {
    }

    /**
     * @brief Construct a new @p unexpected by direct initializing the error object from @p il and @p args
     */
    template<
        typename U,
        typename... Args,
        std::enable_if_t<std::is_constructible_v<ErrorType, std::initializer_list<U>, Args...>> * = nullptr>
    constexpr explicit unexpected(std::in_place_t, std::initializer_list<U> il, Args &&... args)
        : m_error(il, std::forward<Args>(args)...)
    {
    }

    /**
     * @brief Construct a new @p unexpected by direct initializing the error object from @p error
     */
    template<
        typename Err = ErrorType,
        std::enable_if_t<std::is_constructible_v<ErrorType, Err> &&
                         !std::is_same_v<std::remove_cv_t<std::remove_reference_t<Err>>, std::in_place_t> &&
                         !std::is_same_v<std::remove_cv_t<std::remove_reference_t<Err>>, unexpected>> * = nullptr>
    constexpr explicit unexpected(Err && error)
        : m_error(std::forward<Err>(error))
    {
    }

    /**
     * @brief Construct a new @p unexpected by copying the value of another @p unexpected of different error type
     */
    template<
        typename Err,
        std::enable_if_t<!(
            std::is_constructible_v<ErrorType, Err> &&
            !std::is_constructible_v<ErrorType, unexpected<Err> &> &&
            !std::is_constructible_v<ErrorType, unexpected<Err>> &&
            !std::is_constructible_v<ErrorType, unexpected<Err> const &> &&
            !std::is_constructible_v<ErrorType, unexpected<Err> const> &&
            !std::is_convertible_v<unexpected<Err> &, ErrorType> &&
            !std::is_convertible_v<unexpected<Err>, ErrorType> &&
            !std::is_convertible_v<unexpected<Err> const &, ErrorType> &&
            !std::is_convertible_v<unexpected<Err> const, ErrorType>)> * = nullptr,
        std::enable_if_t<!std::is_convertible_v<Err, ErrorType>> * = nullptr>
    constexpr explicit unexpected(unexpected<Err> const & error)
        : m_error(error.m_error)
    {
    }

    /**
     * @brief Construct a new @p unexpected by copying the value of another @p unexpected of different error type
     */
    template<
        typename Err,
        std::enable_if_t<!(
            std::is_constructible_v<ErrorType, Err> &&
            !std::is_constructible_v<ErrorType, unexpected<Err> &> &&
            !std::is_constructible_v<ErrorType, unexpected<Err>> &&
            !std::is_constructible_v<ErrorType, unexpected<Err> const &> &&
            !std::is_constructible_v<ErrorType, unexpected<Err> const> &&
            !std::is_convertible_v<unexpected<Err> &, ErrorType> &&
            !std::is_convertible_v<unexpected<Err>, ErrorType> &&
            !std::is_convertible_v<unexpected<Err> const &, ErrorType> &&
            !std::is_convertible_v<unexpected<Err> const, ErrorType>)> * = nullptr,
        std::enable_if_t<std::is_convertible_v<Err, ErrorType>> * = nullptr>
    constexpr unexpected(unexpected<Err> const & error)
        : m_error(error.m_error)
    {
    }

    /**
     * @brief Construct a new @p unexpected by moving the value of another @p unexpected of different error type
     */
    template<
        typename Err,
        std::enable_if_t<!(
            std::is_constructible_v<ErrorType, Err> &&
            !std::is_constructible_v<ErrorType, unexpected<Err> &> &&
            !std::is_constructible_v<ErrorType, unexpected<Err>> &&
            !std::is_constructible_v<ErrorType, unexpected<Err> const &> &&
            !std::is_constructible_v<ErrorType, unexpected<Err> const> &&
            !std::is_convertible_v<unexpected<Err> &, ErrorType> &&
            !std::is_convertible_v<unexpected<Err>, ErrorType> &&
            !std::is_convertible_v<unexpected<Err> const &, ErrorType> &&
            !std::is_convertible_v<unexpected<Err> const, ErrorType>)> * = nullptr,
        std::enable_if_t<!std::is_convertible_v<Err, ErrorType>> * = nullptr>
    constexpr explicit unexpected(unexpected<Err> && error)
        : m_error(std::move(error.m_error))
    {
    }

    /**
     * @brief Construct a new @p unexpected by moving the value of another @p unexpected of different error type
     */
    template<
        typename Err,
        std::enable_if_t<!(
            std::is_constructible_v<ErrorType, Err> &&
            !std::is_constructible_v<ErrorType, unexpected<Err> &> &&
            !std::is_constructible_v<ErrorType, unexpected<Err>> &&
            !std::is_constructible_v<ErrorType, unexpected<Err> const &> &&
            !std::is_constructible_v<ErrorType, unexpected<Err> const> &&
            !std::is_convertible_v<unexpected<Err> &, ErrorType> &&
            !std::is_convertible_v<unexpected<Err>, ErrorType> &&
            !std::is_convertible_v<unexpected<Err> const &, ErrorType> &&
            !std::is_convertible_v<unexpected<Err> const, ErrorType>)> * = nullptr,
        std::enable_if_t<std::is_convertible_v<Err, ErrorType>> * = nullptr>
    constexpr unexpected(unexpected<Err> && error)
        : m_error(std::move(error.m_error))
    {
    }

    /**
     * @brief Get the error value contained in this @p unexpected instance
     */
    constexpr ErrorType const & value() const &
    {
      return m_error;
    }

    /**
     * @brief Get the error value contained in this @p unexpected instance
     */
    constexpr ErrorType & value() &
    {
      return m_error;
    }

    /**
     * @brief Get the error value contained in this @p unexpected instance
     */
    constexpr ErrorType && value() &&
    {
      return std::move(m_error);
    }

    /**
     * @brief Get the error value contained in this @p unexpected instance
     */
    constexpr ErrorType const && value() const &&
    {
      return std::move(m_error);
    }

    /**
     * @brief Swap the error value of this @p unexpected instance with the one of @p other
     */
    void swap(unexpected & other) noexcept(std::is_nothrow_swappable_v<ErrorType>)
    {
      using std::swap;
      swap(m_error, other.m_error);
    }

    template<typename ErrorType1, typename ErrorType2>
    friend constexpr bool operator==(unexpected<ErrorType1> const & lhs, unexpected<ErrorType2> const & rhs);

    template<typename ErrorType1, typename ErrorType2>
    friend constexpr bool operator!=(unexpected<ErrorType1> const & lhs, unexpected<ErrorType2> const & rhs);

    template<
        typename Err,
        std::enable_if_t<std::is_swappable_v<Err>> *>
    friend void swap(unexpected<Err> & lhs, unexpected<Err> & rhs);

  private:
    ErrorType m_error;
  };

  template<typename ErrorType>
  unexpected(ErrorType) -> unexpected<ErrorType>;

  /**
   * @brief Compare two @p unexpected instances for equality
   */
  template<typename ErrorType1, typename ErrorType2>
  constexpr bool operator==(unexpected<ErrorType1> const & lhs, unexpected<ErrorType2> const & rhs)
  {
    return lhs.m_error == rhs.m_error;
  }

  /**
   * @brief Compare two @p unexpected instances for inequality
   */
  template<typename ErrorType1, typename ErrorType2>
  constexpr bool operator!=(unexpected<ErrorType1> const & lhs, unexpected<ErrorType2> const & rhs)
  {
    return lhs.m_error != rhs.m_error;
  }

  /**
   * @brief Swap the error values of two @p unexpected instances
   */
  template<
      typename Err,
      std::enable_if_t<std::is_swappable_v<Err>> * = nullptr>
  void swap(unexpected<Err> & lhs, unexpected<Err> & rhs)
  {
    lhs.swap(rhs);
  }

  /**
   * @brief A tag type for @p unexpected
   */
  struct unexpect_t
  {
    explicit unexpect_t() = default;
  };

  /**
   * @brief A tap for @p unexpected
   */
  inline constexpr unexpect_t unexpect{};

}  // namespace wanda::std_ext

#endif