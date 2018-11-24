#ifndef WANDA_setting_HPP
#define WANDA_setting_HPP

#include <gio/gio.h>

#include <cstddef>
#include <optional>
#include <string>
#include <type_traits>

#include "type_wrapper.hpp"
#include "variant.hpp"
#include <iostream>

namespace wanda {

struct setting;
using schema = type_wrapper<std::string, struct SchemaTag>;
using key = type_wrapper<std::string, struct KeyTag>;

namespace literals {
key operator""_key(char const * str, std::size_t len);
std::optional<setting> operator""_setting(char const * str, std::size_t len);
}

struct setting {
    ~setting();

    setting(setting const & other);

    template<typename TargetType> std::optional<TargetType> get(key key) const {
        auto value = variant{g_settings_get_value(m_value, key.get().c_str())};
        return value.get<TargetType>();
    }

private:
    explicit setting(schema schema);

    GSettings * m_value;

    friend std::optional<setting> literals::operator""_setting(char const *, std::size_t);
};


}


#endif