#include "test_suite_xdg.hpp"
#include "xdg.hpp"

#include "cute/cute.h"

#include <unistd.h>

#include <filesystem>

namespace wanda
{

namespace
{
    char const * home_only_environment[2] = {"HOME=/home/cute"};
    char const * xdg_data_home_environment[2] = {"XDG_DATA_HOME=/home/cute/xdg_data_home"};
    char const * xdg_config_home_environment[2] = {"XDG_CONFIG_HOME=/home/cute/xdg_config_home"};
    char const * xdg_cache_home_environment[2] = {"XDG_CACHE_HOME=/home/cute/xdg_cache_home"};
    char const * xdg_runtime_dir_environment[2] = {"XDG_RUNTIME_DIR=/home/cute/xdg_runtime_dir"};
}

void test_xdg_variables()
{
    ASSERT_EQUAL("XDG_DATA_HOME", xdg_variable(xdg_directory::data_home));
    ASSERT_EQUAL("XDG_CONFIG_HOME", xdg_variable(xdg_directory::config_home));
    ASSERT_EQUAL("XDG_CACHE_HOME", xdg_variable(xdg_directory::cache_home));
    ASSERT_EQUAL("XDG_RUNTIME_DIR", xdg_variable(xdg_directory::runtime_dir));
}

void test_xdg_path_for_data_home_without_xdg_data_home_in_environment()
{
    auto env = environment{home_only_environment};
    ASSERT_EQUAL("/home/cute/.local/share", xdg_path_for(xdg_directory::data_home, env));
}

void test_xdg_path_for_data_home_with_xdg_data_home_in_environment()
{
    auto env = environment{xdg_data_home_environment};
    ASSERT_EQUAL("/home/cute/xdg_data_home", xdg_path_for(xdg_directory::data_home, env));
}

void test_xdg_path_for_config_home_without_xdg_config_home_in_environment()
{
    auto env = environment{home_only_environment};
    ASSERT_EQUAL("/home/cute/.config", xdg_path_for(xdg_directory::config_home, env));
}

void test_xdg_path_for_config_home_with_xdg_config_home_in_environment()
{
    auto env = environment{xdg_config_home_environment};
    ASSERT_EQUAL("/home/cute/xdg_config_home", xdg_path_for(xdg_directory::config_home, env));
}

void test_xdg_path_for_cache_home_without_xdg_cache_home_in_environment()
{
    auto env = environment{home_only_environment};
    ASSERT_EQUAL("/home/cute/.cache", xdg_path_for(xdg_directory::cache_home, env));
}

void test_xdg_path_for_cache_home_with_xdg_cache_home_in_environment()
{
    auto env = environment{xdg_cache_home_environment};
    ASSERT_EQUAL("/home/cute/xdg_cache_home", xdg_path_for(xdg_directory::cache_home, env));
}

void test_xdg_path_for_runtime_dir_without_xdg_runtime_dir_in_environment()
{
    auto env = environment{home_only_environment};
    auto expected = std::filesystem::path{"/run/user"} / std::to_string(::getuid());
    ASSERT_EQUAL(expected, xdg_path_for(xdg_directory::runtime_dir, env));
}

void test_xdg_path_for_runtime_dir_with_xdg_runtime_dir_in_environment()
{
    auto env = environment{xdg_runtime_dir_environment};
    ASSERT_EQUAL("/home/cute/xdg_runtime_dir", xdg_path_for(xdg_directory::runtime_dir, env));
}

std::pair<cute::suite, std::string> test_suite_xdg()
{
    return std::make_pair(cute::suite{
                              CUTE(test_xdg_variables),
                              CUTE(test_xdg_path_for_data_home_without_xdg_data_home_in_environment),
                              CUTE(test_xdg_path_for_data_home_with_xdg_data_home_in_environment),
                              CUTE(test_xdg_path_for_config_home_without_xdg_config_home_in_environment),
                              CUTE(test_xdg_path_for_config_home_with_xdg_config_home_in_environment),
                              CUTE(test_xdg_path_for_cache_home_without_xdg_cache_home_in_environment),
                              CUTE(test_xdg_path_for_cache_home_with_xdg_cache_home_in_environment),
                              CUTE(test_xdg_path_for_runtime_dir_without_xdg_runtime_dir_in_environment),
                              CUTE(test_xdg_path_for_runtime_dir_with_xdg_runtime_dir_in_environment),
                          },
                          "XDG Utilities");
}

} // namespace wanda