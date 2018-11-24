#ifndef WANDA_VARIANT_HPP
#define WANDA_VARIANT_HPP

#include <gio/gio.h>

#include <optional>
#include <string>
#include <type_traits>

namespace wanda {

struct variant {

    explicit variant(GVariant * variant);

    ~variant();

    template<typename Type> std::optional<Type> get() const {
        if(!m_value) {
            return std::nullopt;
        }

        if constexpr(std::is_same_v<Type, bool>) {
            if(is_of_type(G_VARIANT_TYPE_BOOLEAN)) {
                return static_cast<bool>(g_variant_get_boolean(m_value));
            }
        } else if constexpr(std::is_same_v<Type, std::string>) {
            if(is_of_type(G_VARIANT_TYPE_STRING)) {
                auto length = gsize{};
                auto value = g_variant_get_string(m_value, &length);
                return std::string(value, static_cast<std::size_t>(length));
            }
        }

        return std::nullopt;
    }


private:
    bool is_of_type(GVariantType const * const type) const;

    GVariant * const m_value;
};

}

#endif