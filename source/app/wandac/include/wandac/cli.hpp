#ifndef WANDA_APP_WANDAC_CLI_HPP
#define WANDA_APP_WANDAC_CLI_HPP

#include <lyra/args.hpp>
#include <lyra/cli_parser.hpp>

#include <iosfwd>
#include <string>

namespace wandac
{

  struct cli
  {
    auto parse(lyra::args arguments, std::ostream & error_stream) -> bool;
    auto print_usage(std::ostream & output_stream) -> void;

    std::string command{};
    bool help{};

  private:
    lyra::cli_parser parser{};
  };

}  // namespace wandac

#endif