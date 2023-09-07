#include "wandac/cli.hpp"

#include <lyra/arg.hpp>
#include <lyra/help.hpp>

#include <ostream>

namespace wandac
{

  auto cli::parse(lyra::args arguments, std::ostream & error_stream) -> bool
  {
    parser |=  //
        lyra::help(help) |  //
        lyra::arg{command, "command"}("The command to send to the deamon").required();

    auto result = parser.parse(arguments);

    if (!result)
    {
      error_stream << "Error while processing command line arguments: " << result.message() << '\n' << parser << '\n';
      return false;
    }

    return true;
  }

  auto cli::print_usage(std::ostream & output_stream) -> void { output_stream << parser << '\n'; }

}  // namespace wandac
