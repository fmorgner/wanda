/**
 * @file    setting.hpp
 * @author  Felix Morgner (felix.morgner@gmail.com)
 * @since   1.0.0
 */

#ifndef WANDA_setting_HPP
#define WANDA_setting_HPP

#include "wanda/deferred_failure.hpp"
#include "wanda/type_wrapper.hpp"

#include <gio/gio.h>

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <variant>
#include <vector>

namespace wanda
{
  struct setting;

  /**
   * @brief A convenience type to represent setting keys
   */
  using key = type_wrapper<std::string, struct KeyTag>;

  namespace literals
  {
    /**
     * @brief UDL to create setting keys
     */
    key operator""_key(char const * str, std::size_t len);

    /**
     * @brief UDL to create setting schemas
     */
    std::optional<setting> operator""_setting(char const * str, std::size_t lent);
  }  // namespace literals

  /**
   * @brief A simple wrapper for GSettings Schemas
   */
  struct setting
  {
    struct entry
    {
      using value_type = std::variant<std::monostate, bool, std::int32_t, std::int64_t, std::uint32_t, std::uint64_t, double, std::string, std::vector<std::string>>;

      /**
       * @brief Get the value of the settings entry
       */
      value_type operator*() const;

      /**
       * @brief Assign the given @p value to the settings entry
       * 
       * @returns @p true iff. the value could be successfully assigned
       */
      template<typename Type>
      bool operator=(Type value)
      {
        struct setting_applier
        {
          setting_applier(GSettings * setting, gchar const * key, Type value) noexcept
              : m_result{[&] {
                if constexpr (std::is_same_v<Type, bool>)
                {
                  return g_settings_set_boolean(setting, key, value);
                }
                else if constexpr (std::is_same_v<Type, std::int32_t>)
                {
                  return g_settings_set_int(setting, key, value);
                }
                else if constexpr (std::is_same_v<Type, std::int64_t>)
                {
                  return g_settings_set_int64(setting, key, value);
                }
                else if constexpr (std::is_same_v<Type, std::uint32_t>)
                {
                  return g_settings_set_uint(setting, key, value);
                }
                else if constexpr (std::is_same_v<Type, std::uint64_t>)
                {
                  return g_settings_set_uint64(setting, key, value);
                }
                else if constexpr (std::is_same_v<Type, double>)
                {
                  return g_settings_set_double(setting, key, value);
                }
                else if constexpr (std::is_same_v<Type, std::string>)
                {
                  return g_settings_set_string(setting, key, value.c_str());
                }
                else if constexpr (std::is_same_v<Type, std::vector<std::string>>)
                {
                  auto temp = std::vector<gchar const *>{value.size() + 1};
                  std::transform(value.begin(), value.end(), temp.begin(), [](auto const & str) { return str.c_str(); });
                  return g_settings_set_strv(setting, key, temp.data());
                }
              }()}
          {
          }

          ~setting_applier()
          {
            g_settings_sync();
          }

          operator bool() const
          {
            return m_result;
          }

        private:
          gboolean const m_result;
        };

        return setting_applier{m_settings.get(), m_key.get().c_str(), value};
      }

    private:
      entry(setting const & schema, key key);

      std::unique_ptr<GSettings, decltype(&g_object_unref)> m_settings;

      key m_key;

      friend setting;
    };

    /**
     * @brief Get the entry for the given key
     * 
     * @return An <code>std::optional</code> wrapping the entry associated with
     * the given key, or an empty <code>std::optional</code> if the desired key
     * does not exist in the setting's schema.
     */
    std::optional<entry> operator[](key key) const;

  private:
    explicit setting(GSettingsSchema * schema);

    std::unique_ptr<GSettingsSchema, decltype(&g_settings_schema_unref)> m_schema;

    friend std::optional<setting> literals::operator""_setting(char const *, std::size_t);
  };

}  // namespace wanda

#endif