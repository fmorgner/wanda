#include "setting.hpp"

#include <algorithm>
#include <type_traits>

namespace wanda
{

// UDL implementations

key literals::operator""_key(char const *str, std::size_t len)
{
    return key{{str, len}};
}

std::optional<setting> literals::operator""_setting(char const *str, std::size_t len)
{
    auto source = g_settings_schema_source_get_default();
    if (!source)
    {
        return std::nullopt;
    }

    auto schema = g_settings_schema_source_lookup(source, str, true);
    if (!schema)
    {
        return std::nullopt;
    }

    return setting{schema};
}

// 'setting' implementation

setting::setting(GSettingsSchema *schema)
    : m_schema{schema, &g_settings_schema_unref}
{
}

std::optional<setting::entry> setting::operator[](key key) const
{
    if (!g_settings_schema_has_key(m_schema.get(), key.get().c_str()))
    {
        return std::nullopt;
    }

    return setting::entry{*this, std::move(key)};
}

// 'setting::entry' implementation

setting::entry::entry(setting const &setting, key key)
    : m_settings{g_settings_new(g_settings_schema_get_id(setting.m_schema.get())), &g_object_unref}, m_key{key.get()}
{
}

setting::entry::value_type setting::entry::operator*() const
{
    auto value = std::unique_ptr<GVariant, decltype(&g_variant_unref)>{g_settings_get_value(m_settings.get(), m_key.get().c_str()), &g_variant_unref};
    auto raw = value.get();

    if (g_variant_is_of_type(raw, G_VARIANT_TYPE_BOOLEAN))
    {
        return static_cast<bool>(g_variant_get_boolean(raw));
    }
    else if (g_variant_is_of_type(raw, G_VARIANT_TYPE_INT32))
    {
        return static_cast<std::int32_t>(g_variant_get_int32(raw));
    }
    else if (g_variant_is_of_type(raw, G_VARIANT_TYPE_INT64))
    {
        return static_cast<std::int64_t>(g_variant_get_int64(raw));
    }
    else if (g_variant_is_of_type(raw, G_VARIANT_TYPE_UINT32))
    {
        return static_cast<std::uint32_t>(g_variant_get_uint32(raw));
    }
    else if (g_variant_is_of_type(raw, G_VARIANT_TYPE_UINT64))
    {
        return static_cast<std::uint64_t>(g_variant_get_uint64(raw));
    }
    else if (g_variant_is_of_type(raw, G_VARIANT_TYPE_DOUBLE))
    {
        return static_cast<double>(g_variant_get_double(raw));
    }
    else if (g_variant_is_of_type(raw, G_VARIANT_TYPE_STRING))
    {
        auto size = gsize{};
        auto string = g_variant_get_string(raw, &size);
        return std::string{string, size};
    }
    else if (g_variant_is_of_type(raw, G_VARIANT_TYPE_STRING_ARRAY))
    {
        auto length = gsize{};
        auto data = g_variant_get_strv(raw, &length);
        return std::vector<std::string>{data, data + length};
    }

    return {};
}

} // namespace wanda