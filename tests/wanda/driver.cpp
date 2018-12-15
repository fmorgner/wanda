#include "cute/cute.h"
#include "cute/cute_runner.h"
#include "cute/tap_listener.h"

#include <algorithm>
#include <iostream>
#include <iterator>
#include <string>
#include <utility>
#include <vector>

namespace wanda::test
{
    std::pair<cute::suite, std::string> suite();
}

int main(int argc, char const *const *argv)
{
    auto listener = cute::tap_listener<>{std::cout};
    auto runner = cute::makeRunner(listener, argc, argv);
    auto suite = wanda::test::suite();
    return !runner(suite.first, suite.second.c_str());
}