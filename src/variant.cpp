#include "variant.hpp"

namespace wanda {

variant::variant(GVariant * value) : m_value{value} { }

variant::~variant() {
    if(m_value) {
        g_variant_unref(m_value);
    }
}

bool variant::is_of_type(GVariantType const * const type) const {
    return g_variant_is_of_type(m_value, type);
}

}