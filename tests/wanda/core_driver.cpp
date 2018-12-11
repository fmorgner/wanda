#include "test_suite_xdg.hpp"

#include "cute/cute.h"
#include "cute/cute_runner.h"
#include "cute/tap_listener.h"

#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>

int main(int argc, char const *const *argv)
{
    auto listener = cute::tap_listener<>{std::cout};
    auto runner = cute::makeRunner(listener, argc, argv);
    auto suites = std::vector<std::pair<cute::suite, std::string>>{wanda::test_suite_xdg()};
    return !all_of(cbegin(suites), cend(suites), [&](auto const &suite) { return runner(suite.first, suite.second.c_str()); });
}