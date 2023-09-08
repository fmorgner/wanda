#include "wandac/cli.hpp"

#include <catch2/catch_all.hpp>
#include <catch2/catch_test_macros.hpp>
#include <lyra/args.hpp>

#include <sstream>
#include <string>

using namespace std::string_literals;

namespace wanda::tests::app::wandac
{

  template<typename ...Ts>
  auto make_argument_list(Ts const & ... args) -> lyra::args
  {
    return {"wanda"s, static_cast<std::string>(args)...};
  }

  SCENARIO("Empty argument list parsing", "[app][client][cli]")
  {
    GIVEN("A fresh cli instance and error stream")
    {
      auto cli = ::wandac::cli{};
      auto error_stream = std::ostringstream{};

      WHEN("invoking parse without any program arguments")
      {
        auto result = cli.parse(make_argument_list(), error_stream);

        THEN("the return value is false") { REQUIRE_FALSE(result); }
        THEN("the error stream is not empty") { REQUIRE_FALSE(error_stream.view().empty()); }
        THEN("the help flag is not set") { REQUIRE_FALSE(cli.help); }
        THEN("the command is empty") { REQUIRE(cli.command.empty()); }
      }
    }
  }

  SCENARIO("Valid argument list parsing", "[app][client][cli]")
  {
    GIVEN("A fresh cli instance and error stream")
    {
      auto cli = ::wandac::cli{};
      auto error_stream = std::ostringstream{};

      AND_GIVEN("'-h' in the argument list")
      {
        auto argument_list = make_argument_list("-h");

        WHEN("invoking parse without additional arguments")
        {
          auto result = cli.parse(argument_list, error_stream);

          THEN("the return value is true") { REQUIRE(result); }
          THEN("the error stream is empty") { REQUIRE(error_stream.view().empty()); }
          THEN("the help flag is set") { REQUIRE(cli.help); }
          THEN("the command is empty") { REQUIRE(cli.command.empty()); }
        }
      }

      AND_GIVEN("'change' in the argument list")
      {
        auto argument_list = make_argument_list("change");

        WHEN("invoking parse without additional arguments")
        {
          auto result = cli.parse(argument_list, error_stream);

          THEN("the return valis is true") { REQUIRE(result); }
          THEN("the error stream is empty") { REQUIRE(error_stream.view().empty()); }
          THEN("the help flag is not set") { REQUIRE_FALSE(cli.help); }
          THEN("the command is not empty") { REQUIRE_FALSE(cli.command.empty()); }
          THEN("the command is 'change'") { REQUIRE(cli.command == "change"); }
        }
      }
    }
  }

}  // namespace wanda::tests::app::wandac
