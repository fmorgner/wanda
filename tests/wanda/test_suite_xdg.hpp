#ifndef WANDA_TEST_SUITE_XDG_HPP
#define WANDA_TEST_SUITE_XDG_HPP

#include "cute/cute.h"

#include <string>
#include <utility>

namespace wanda
{

std::pair<cute::suite, std::string> test_suite_xdg();

} // namespace wanda

#endif