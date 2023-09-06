#include "wanda/control/commander.hpp"

#include "wanda/proto/message.hpp"
#include "wanda/proto/version.hpp"
#include "wanda/std_ext/optional.hpp"
#include "wanda/system/logging.hpp"

#include <boost/asio/io_context.hpp>
#include <spdlog/fmt/ostr.h>

namespace wanda::control
{
  commander::commander(boost::asio::io_context & service, std::filesystem::path socket, listener & listener)
      : m_service{service}
      , m_endpoint{socket.string()}
      , m_socket{service}
      , m_listener{listener}
  {
  }

  void commander::start()
  {
    m_socket.async_connect(m_endpoint, [&](auto const & error) {
      if (error)
      {
        system::get_logger()->error("error while connecting to control interface: '{}'", error.message());
      }
      else
      {
        system::get_logger()->info("establishing connection to wanda deamon");
        m_connection = wanda::control::make_connection(std::move(m_socket));
        m_connection->add(this);
        m_connection->start();
        m_connection->send({proto::message_source_controller, proto::message_command_hello, proto::version});
      }
    });
  }

  void commander::stop()
  {
    system::get_logger()->info("closing control connection");
    m_connection->close();
  }

  void commander::send(proto::command command)
  {
    using namespace wanda::std_ext;

    if (!m_connection || m_connection->current_state() != connection::state::established)
    {
      system::get_logger()->error("tried to send command without an established connection");
      m_listener.on_error(*this, "tried to send command without an established connection");
      return;
    }

    with(command.message(), [&](auto const & message) { m_connection->send(message); }) ||
        [&] { system::get_logger()->error("unknown command"); };
  }

  void commander::on_error(connection::pointer connection, std::error_code error)
  {
    system::get_logger()->error("control interface communication error: '{}'", error.message());
  }

  void commander::on_received(connection::pointer connection, proto::message message)
  {
    if (auto state = connection->current_state(); message.command == "HELLO" && state == connection::state::fresh)
    {
      system::get_logger()->info("connection to wanda deamon successfully established");
      connection->update(connection::state::established);
      m_listener.on_connected(*this);
    }
    else
    {
      system::get_logger()->error("unexpected message: '{}'", message);
      m_listener.on_error(*this, "unexpected message '" + static_cast<std::string>(message) + '\'');
    }
  }

}  // namespace wanda::control