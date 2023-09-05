#include "wanda/system/xdg.hpp"

#include "wanda/system/environment.hpp"

#include <catch2/catch_all.hpp>
#include <unistd.h>

#include <filesystem>
#include <string>
#include <utility>

using namespace std::string_literals;

namespace wanda::system::test
{

  TEST_CASE("XDG variable names match the specification", "[system][xdg][spec]")
  {
    REQUIRE(xdg_variable(xdg_directory::data_home) == "XDG_DATA_HOME");
    REQUIRE(xdg_variable(xdg_directory::config_home) == "XDG_CONFIG_HOME");
    REQUIRE(xdg_variable(xdg_directory::cache_home) == "XDG_CACHE_HOME");
    REQUIRE(xdg_variable(xdg_directory::runtime_dir) == "XDG_RUNTIME_DIR");
  }

  SCENARIO("The values of the XDG variables depend on the environment", "[system][xdg][spec]")
  {
    auto const home_directory = std::filesystem::path{"/home/test"};
    auto const run_directory = std::filesystem::path{"/run/user"};
    auto const uid = std::to_string(::getuid());

    GIVEN("An environment that only contains HOME")
    {
      auto home = "HOME="s + home_directory.native();
      char const * env_data[] = {home.c_str(), nullptr};
      auto env = environment{env_data};

      THEN("the data home is '<home_directory>/.local/share")
      {
        REQUIRE(xdg_path_for(xdg_directory::data_home, env) == home_directory / ".local/share");
      }

      THEN("the config home is '<home_directory>/.config")
      {
        REQUIRE(xdg_path_for(xdg_directory::config_home, env) == home_directory / ".config");
      }

      THEN("the cache home is '<home_directory>/.cache")
      {
        REQUIRE(xdg_path_for(xdg_directory::cache_home, env) == home_directory / ".cache");
      }

      THEN("the runtime directory is '<run_directory>/<uid>'")
      {
        REQUIRE(xdg_path_for(xdg_directory::runtime_dir, env) == run_directory / uid);
      }
    }

    GIVEN("An environment that only contains HOME and XDG_DATA_HOME")
    {
      auto home = "HOME="s + home_directory.native();
      auto data_home = "XDG_DATA_HOME=/home/test/xdg_data_home"s;
      char const * env_data[] = {home.c_str(), data_home.c_str(), nullptr};
      auto env = environment{env_data};

      THEN("the data home is '$XDG_DATA_HOME")
      {
        REQUIRE(xdg_path_for(xdg_directory::data_home, env) == env["XDG_DATA_HOME"]);
      }

      THEN("the config home is '<home_directory>/.config")
      {
        REQUIRE(xdg_path_for(xdg_directory::config_home, env) == home_directory / ".config");
      }

      THEN("the cache home is '<home_directory>/.cache")
      {
        REQUIRE(xdg_path_for(xdg_directory::cache_home, env) == home_directory / ".cache");
      }

      THEN("the runtime directory is '<run_directory>/<uid>'")
      {
        REQUIRE(xdg_path_for(xdg_directory::runtime_dir, env) == run_directory / uid);
      }
    }

    GIVEN("An environment that only contains HOME and XDG_CONFIG_HOME")
    {
      auto home = "HOME="s + home_directory.native();
      auto config_home = "XDG_CONFIG_HOME=/home/test/xdg_config_home"s;
      char const * env_data[] = {home.c_str(), config_home.c_str(), nullptr};
      auto env = environment{env_data};

      THEN("the data home is '<home_directory>/.local/share")
      {
        REQUIRE(xdg_path_for(xdg_directory::data_home, env) == home_directory / ".local/share");
      }

      THEN("the config home is '$XDG_CONFIG_HOME")
      {
        REQUIRE(xdg_path_for(xdg_directory::config_home, env) == env["XDG_CONFIG_HOME"]);
      }

      THEN("the cache home is '<home_directory>/.cache")
      {
        REQUIRE(xdg_path_for(xdg_directory::cache_home, env) == home_directory / ".cache");
      }

      THEN("the runtime directory is '<run_directory>/<uid>'")
      {
        REQUIRE(xdg_path_for(xdg_directory::runtime_dir, env) == run_directory / uid);
      }
    }

    GIVEN("An environment that only contains HOME and XDG_CACHE_HOME")
    {
      auto home = "HOME="s + home_directory.native();
      auto cache_home = "XDG_CACHE_HOME=/home/test/xdg_cache_home"s;
      char const * env_data[] = {home.c_str(), cache_home.c_str(), nullptr};
      auto env = environment{env_data};

      THEN("the data home is '<home_directory>/.local/share")
      {
        REQUIRE(xdg_path_for(xdg_directory::data_home, env) == home_directory / ".local/share");
      }

      THEN("the cache home is '<home_directory>/.config")
      {
        REQUIRE(xdg_path_for(xdg_directory::config_home, env) == home_directory / ".config");
      }

      THEN("the config home is '$XDG_CACHE_HOME")
      {
        REQUIRE(xdg_path_for(xdg_directory::cache_home, env) == env["XDG_CACHE_HOME"]);
      }

      THEN("the runtime directory is '<run_directory>/<uid>'")
      {
        REQUIRE(xdg_path_for(xdg_directory::runtime_dir, env) == run_directory / uid);
      }
    }

    GIVEN("An environment that only contains HOME and XDG_RUNTIME_DIR")
    {
      auto home = "HOME="s + home_directory.native();
      auto runtime_dir = "XDG_RUNTIME_DIR=/home/test/xdg_runtime_dir"s;
      char const * env_data[] = {home.c_str(), runtime_dir.c_str(), nullptr};
      auto env = environment{env_data};

      THEN("the data home is '<home_directory>/.local/share")
      {
        REQUIRE(xdg_path_for(xdg_directory::data_home, env) == home_directory / ".local/share");
      }

      THEN("the config home is '<home_directory>/.config")
      {
        REQUIRE(xdg_path_for(xdg_directory::config_home, env) == home_directory / ".config");
      }

      THEN("the cache home is '<home_directory>/.cache")
      {
        REQUIRE(xdg_path_for(xdg_directory::cache_home, env) == home_directory / ".cache");
      }

      THEN("the runtime directory is '$XDG_RUNTIME_DIR'")
      {
        REQUIRE(xdg_path_for(xdg_directory::runtime_dir, env) == env["XDG_RUNTIME_DIR"]);
      }
    }
  }

}  // namespace wanda::system::test