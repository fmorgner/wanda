#include "wandac/listener.hpp"

#include "wanda/control/commander.hpp"
#include "wandac/cli.hpp"

#include <wanda/control/commander.hpp>
#include <wanda/proto/command.hpp>

#include <boost/asio/io_context.hpp>

namespace wandac
{

  listener::listener(wandac::cli const & cli, boost::asio::io_context & service)
      : m_cli{cli}
      , m_service{service}
  {
  }

  auto listener::on_connected(wanda::control::commander & commander) -> void
  {
    if (m_cli.command == "change")
    {
      commander.send(wanda::proto::make_change_command());

      post(m_service, [&] { commander.stop(); });
    }
  }

}  // namespace wandac
