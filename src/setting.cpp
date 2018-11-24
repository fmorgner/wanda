#include "setting.hpp"

namespace wanda {

namespace literals {
    key operator""_key(char const * str, std::size_t len) {
        return key{{str, len}};
    }

    std::optional<setting> operator""_setting(char const * str, std::size_t len) {
        auto source = g_settings_schema_source_get_default();
        auto schema = g_settings_schema_source_lookup(source, str, true);
        if(schema) {
            g_settings_schema_unref(schema);
            return setting{::wanda::schema{{str, len}}};
        }
        return std::nullopt;
    }
}

setting::setting(schema schema) : m_value{g_settings_new(schema.get().c_str())} { }

setting::setting(setting const & other) : m_value{reinterpret_cast<GSettings *>(g_object_ref(other.m_value))} { }

setting::~setting() {
    g_clear_object(&m_value);
}

}